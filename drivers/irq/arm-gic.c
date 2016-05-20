#include <irqchip.h>
#include <irqdesc.h>
#include <interrupt.h>
#include <io.h>
#include <init.h>
#include <soc/irq.h>
#include <smp/ipi.h>
#include <errno.h>
#include "arm-gic.h"


struct gic_intctrl {
	struct irqchip	chip;
	struct irqdesc  descs[GIC_NBR_IRQ];
	void __iomem	*cpu_base;
};



static void __iomem *gic_get_cpubase(struct gic_intctrl *gic)
{
	void __iomem *cpu_base = gic->cpu_base;

	return cpu_base;
}


static void gic_irq_mask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GICD_ICENABLER(irq/32), 1 << (irq % 32));
}

static void gic_irq_unmask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GICD_ISENABLER(irq/32), 1 << (irq % 32));
}

static void gic_irq_eoi(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	struct gic_intctrl *gic = container_of(chip, struct gic_intctrl, chip);

	iowrite32(gic_get_cpubase(gic) + GICC_EOIR, desc->irq);
}

static void gic_set_affinity_cpu(struct gic_intctrl *gic, uint cpu)
{
	void __iomem *dist_base = gic->chip.iobase;
	int irq_nbr = gic->chip.irq_nbr;
	u32 msk = 1 << cpu;
	int i;

	// each regs has info for 4 irqs
	msk |= msk << 8;
	msk |= msk << 16;

	for (i = 32; i < irq_nbr; i += 4)
		ioset32(dist_base + GICD_ITARGETSR(i/4), msk);
}

static void gic_init_private(struct gic_intctrl *gic, uint cpu)
{
	void __iomem *dist_base = gic->chip.iobase;
	void __iomem *cpu_base = gic_get_cpubase(gic);
	uint cpu_ctrl;
	int i;

	// disable PPIs & enable SGIs
	iowrite32(dist_base + GICD_ICENABLER(0), GIC_PPI_MASK);
	iowrite32(dist_base + GICD_ISENABLER(0), GIC_SGI_MASK);

	// Set priority on PPI and SGI interrupts
	for (i = 0; i < 32; i += 4)
		iowrite32(dist_base + GICD_IPRIOR(i/4), GIC_PRIO_DEFAULT * 0x01010101U);

	iowrite32(cpu_base + GICC_PMR, GIC_PRIO_FILTER);

	if (cpu == 0)
		gic_set_affinity_cpu(gic, cpu);

	// enable the CPU interface
	cpu_ctrl = ioread32(cpu_base + GICC_CTLR);
	cpu_ctrl |= GICC_CTLR_ENABLE;
	iowrite32(cpu_base + GICC_CTLR, cpu_ctrl);
}

static void gic_init_shared(struct gic_intctrl *gic, uint irq_nbr)
{
	void __iomem *dist_base = gic->chip.iobase;
	uint i;

	iowrite32(dist_base + GICD_CTLR, GICD_CTLR_DISABLE);

	for (i = 32; i < irq_nbr; i += 16)
		iowrite32(dist_base + GICD_ICFGR(i/16), GICD_ICFGR_LEVEL * 0x55555555U);

	for (i = 32; i < irq_nbr; i += 4)
		iowrite32(dist_base + GICD_IPRIOR(i/4), GIC_PRIO_DEFAULT * 0x01010101U);

	for (i = 32; i < irq_nbr; i += 32)
		iowrite32(dist_base + GICD_ICENABLER(i/32), 0xFFFFFFFF);

	// PPIs can use the percpu handler
	for (i = 16; i < 32; i++)
		gic->descs[i].handler = irq_handle_percpu;

	iowrite32(dist_base + GICD_CTLR, GICD_CTLR_ENABLE);
}

static struct gic_intctrl gic_intctrl;

static void __init gic_init(void)
{
	struct gic_intctrl *gic = &gic_intctrl;
	struct irqchip *chip = &gic->chip;
	void __iomem *dist_base;
	void __iomem *cpu_base;
	uint irq_nbr;
	uint lines;

	cpu_base = ioremap(GICC_BASE, 0x0100);
	dist_base = ioremap(GICD_BASE, 0x1000);

	gic->cpu_base = cpu_base;
	chip->iobase = dist_base;
	chip->name = "gic";
	chip->default_handler = irq_handle_eoi;
	chip->mask	= gic_irq_mask;
	chip->unmask	= gic_irq_unmask;
	chip->eoi	= gic_irq_eoi;

	lines = ioread32(chip->iobase + GICD_TYPER);
	lines = GICD_TYPER_LINES(lines);
	irq_nbr = (lines + 1) * 32;
	if (irq_nbr > GIC_NBR_IRQ)
		irq_nbr = GIC_NBR_IRQ;
	chip->irq_nbr = irq_nbr;

	gic_init_shared(gic, irq_nbr);
	gic_init_private(gic, 0);

	irqchip_init(NULL, chip);
}
board_irq_initcall(gic_init);


/******************************************************************************/
#include <symbols.h>

static void gic_handle_irq(struct eframe *regs)
{
	struct gic_intctrl *gic = &gic_intctrl;
	struct irqchip *chip = &gic->chip;
	void __iomem *cpu_base = gic_get_cpubase(gic);

//printf("%s(): reached!\n", __func__);
	while (1) {
		u32 iar = ioread32(cpu_base + GICC_IAR);
		u32 irq = GICC_IAR_INTID(iar);
		struct irqdesc *desc = &chip->descs[irq];
//printf("%s(): irq = %d\n", __func__, irq);

		if (likely(irq >= 16 && irq < GIC_MAX_IRQ)) {
			__irq_handler(desc, regs);
		} else if (irq < 16) {
			iowrite32(cpu_base + GICC_EOIR, iar);
#ifdef	CONFIG_SMP
			mbr_smp();	// pair with gic_ipi_send()'s write barrier
			__smp_ipi_process(irq);
#endif
		} else
			return;
	}
}
strong_alias(gic_handle_irq, mach_irq_handler);

/******************************************************************************/
#ifdef	CONFIG_SMP

// FIXME: msg is a bitmap of ipi numbers ...
//	  API to be changed because not generic enough
#define	foreach_ipi(ipi, msg)			\
	for (ipi = 0; msg; ipi++)		\
		if ((msg & (1 << ipi)) == 0)	\
			continue;		\
		else

static int gic_ipi_send(uint cpu, uint msg)
{
	struct gic_intctrl *gic = &gic_intctrl;
	void __iomem *dist_base = gic->chip.iobase;
	u32 val;
	uint ipi;

	if (msg & ~0xffff)
		return -EINVAL;	// support only IPI 0-15

	switch (cpu) {
	case SMP_IPI_SELF:
		val = GICD_SGIR_DEST_SELF;
		break;
	case SMP_IPI_OTHER:
		val = GICD_SGIR_DEST_OTHER;
		break;
	case SMP_IPI_ALL:
		val = GICD_SGIR_DEST_LIST | GICD_SGIR_LIST_ALL;
		break;
	case 0 ... 7:
		val = GICD_SGIR_DEST_LIST | GICD_SGIR_LIST(cpu);
		break;
	default:
		return -EINVAL;
	}

	// we're publishing something here
	mbw_smp();		// pair with gic_handle_irq()'s read barrier

	foreach_ipi(ipi, msg)
		iowrite32(dist_base + GICD_SGIR, val | GICD_SGIR_IPI(ipi));

	return 0;
}

void __smp_ipi_send(uint cpu, uint msg)
{
	gic_ipi_send(cpu, msg);
}
#endif
