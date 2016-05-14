#include <irqchip.h>
#include <irqdesc.h>
#include <interrupt.h>
#include <io.h>
#include <init.h>
#include <soc/irq.h>
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


static void gic_init_private(struct gic_intctrl *gic)
{
	void __iomem *dist_base = gic->chip.iobase;
	void __iomem *cpu_base = gic_get_cpubase(gic);
	uint cpu_ctrl;
	int i;

	// disable all PPIs & all SGIs
	iowrite32(dist_base + GICD_ICENABLER(0), GIC_PPI_MASK|GIC_SGI_MASK);

	// Set priority on PPI and SGI interrupts
	for (i = 0; i < 32; i += 4)
		iowrite32(dist_base + GICD_IPRIOR(i/4), GIC_PRIO_DEFAULT * 0x01010101U);

	iowrite32(cpu_base + GICC_PMR, GIC_PRIO_FILTER);

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

	for (i = 32; i < irq_nbr; i += 4)
		iowrite32(dist_base + GICD_ITARGETSR(i/4), 0x01010101);

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
	chip->irq_nbr = GIC_NBR_IRQ;

	gic_init_shared(gic, irq_nbr);
	gic_init_private(gic);

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
		} else
			return;
	}
}
strong_alias(gic_handle_irq, mach_irq_handler);
