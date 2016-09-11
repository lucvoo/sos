#include <bitops/findbit.h>
#include <smp/initcall.h>
#include <interrupt.h>
#include <arch/gic.h>
#include <arch/cm.h>
#include <barrier.h>
#include <irqchip.h>
#include <smp/ipi.h>
#include <soc/gic.h>
#include <errno.h>
#include <types.h>
#include <init.h>
#include <irq.h>
#include <smp.h>
#include <io.h>

#if BITS_PER_LONG != 32
#error "64bit GIC is not supported yet"
#endif


#define	GIC_OFF_IP_IRQS	(GIC_NBR_SH_IRQS - GIC_NBR_IP_IRQS)
#define	GIC_NBR_VP_IRQS	7

#define	GIC_PIN_SHARED	3
#define	GIC_PIN_LOCAL	2


struct gic_intctrl {
	struct {
		struct irqchip	chip;
		struct irqdesc  descs[GIC_NBR_SH_IRQS];
	} sh;
	struct {
		struct irqchip	chip;
		struct irqdesc  descs[GIC_NBR_VP_IRQS];
	} vp;
};


static void gic_sh_irq_mask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GIC_SH_RMASK(irq/32), 1 << (irq % 32));
}

static void gic_sh_irq_unmask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GIC_SH_SMASK(irq/32), 1 << (irq % 32));
}

static void gic_sh_irq_ack(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;

	iowrite32(chip->iobase + GIC_SH_WEDGE, GIC_SH_WEDGE_CLR | desc->irq);
}


static struct gic_intctrl gic_intctrl;
static void gic_sh_irq_handler(struct irqdesc *__desc)
{
	struct irqchip *chip = &gic_intctrl.sh.chip;
	void __iomem *base = chip->iobase;
	uint irq;

	// FIXME: use SMP affinity

	for (irq = 0; irq < GIC_NBR_SH_IRQS; irq += 32) {
		u32 raw = ioread32(base + GIC_SH_PEND(irq/32));
		u32 msk = ioread32(base + GIC_SH_MASK(irq/32));
		u32 cooked = raw & msk;

		while (cooked) {
			int bit = bitop_fmsb(cooked);

			cooked &= ~(1 << bit);

			irq_handle_desc(&chip->descs[irq + bit]);
		}
	}
}


static void gic_map_irq_to_vpe(struct gic_intctrl *gic, uint irq, uint vpe)
{
	struct irqchip *chip = &gic->sh.chip;
	void __iomem *base = chip->iobase;

	iowrite32(base + GIC_SH_MPIN(irq), GIC_SH_MPIN_PIN(GIC_PIN_SHARED));
	iowrite32(base + GIC_SH_VPE(irq), (1 << vpe));
}


static int gic_sh_init(struct gic_intctrl *gic, void __iomem *gic_base)
{
	void __iomem *base = gic_base + GIC_SH_OFFSET;
	struct irqchip *chip = &gic->sh.chip;
	struct irqdesc *parent;
	uint nbr_irq;
	uint irq;
	u32 cfg;

	cfg = ioread32(base + GIC_SH_CFG);

	// retrieve the number of supported IRQs
	nbr_irq = GIC_SH_CFG_NBRIRQS(cfg);
	if (nbr_irq != GIC_NBR_SH_IRQS) {
		pr_err("nbr irqs doesn't match: %d != %d\n", nbr_irq, GIC_NBR_SH_IRQS);
		return -EINVAL;
	}

	chip->name	= "mips-gic-sh";
	chip->irq_nbr	= nbr_irq;
	chip->iobase	= base;

	chip->default_handler = irq_handle_percpu;
	chip->ack	= gic_sh_irq_ack;
	chip->mask	= gic_sh_irq_mask;
	chip->unmask	= gic_sh_irq_unmask;

	parent = irq_get_desc("cpuintc", GIC_PIN_SHARED);
	irqchip_chain(parent, gic_sh_irq_handler, chip);


	// initialize all irqs to:
	// - level high triggering
	// - masked
	for (irq = 0; irq < nbr_irq; irq += 32) {
		iowrite32(base + GIC_SH_EDGE(irq/32),  0x00000000);
		iowrite32(base + GIC_SH_HIGH(irq/32),  0xffffffff);
		iowrite32(base + GIC_SH_RMASK(irq/32), 0xffffffff);
	}

	// initialize non-ipi irqs to:
	// - map to CPU irq: GIC_PIN_SHARED
	// - affinity: VPE0
	for (irq = 0; irq < (nbr_irq - GIC_NBR_IP_IRQS); irq += 1) {
		gic_map_irq_to_vpe(gic, irq, 0);
	}

	return 0;
}

/******************************************************************************/

static void gic_vp_irq_mask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GIC_VP_RMASK, 1 << (irq % 32));
}

static void gic_vp_irq_unmask(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	uint irq = desc->irq;

	iowrite32(chip->iobase + GIC_VP_SMASK, 1 << (irq % 32));
}


static void gic_vp_irq_handler(struct irqdesc *__desc)
{
	struct irqchip *chip = &gic_intctrl.vp.chip;
	void __iomem *base = chip->iobase;
	u32 raw = ioread32(base + GIC_VP_PEND);
	u32 msk = ioread32(base + GIC_VP_MASK);
	u32 cooked = raw & msk;

	if (!cooked) {
		// FIXME: spurious
		return;
	}

	do {
		int bit = bitop_fmsb(cooked);
		uint irq = 0 + bit;

		irq_handle_desc(&chip->descs[irq]);

		// FIXME: handling of IPIs

		cooked &= ~(1 << bit);
	} while (cooked);
}


static void gic_init_local(struct gic_intctrl *gic, uint cpu)
{
	struct irqchip *chip = &gic->vp.chip;
	void __iomem *lbase = chip->iobase;
	u32 vpe_ctl;

	vpe_ctl = ioread32(lbase + GIC_VP_CTRL);
	(void) vpe_ctl;
	// FIXME: should check which irq is routable before
	// mapping them

	// initialize all irqs to:
	// - masked
	// - mapped to GIC_PIN_LOCAL
	iowrite32(lbase + GIC_VP_RMASK, 0xffffffff);

	iowrite32(lbase + GIC_VP_WD_MAP,    GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_COMP_MAP,  GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_TIMER_MAP, GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_PCI_MAP,   GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_FDC_MAP,   GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_SW0_MAP,   GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
	iowrite32(lbase + GIC_VP_SW1_MAP,   GIC_VP_MPIN_PIN(GIC_PIN_LOCAL));
}

static void gic_vp_init(struct gic_intctrl *gic, void __iomem *gic_base)
{
	void __iomem *base = gic_base + GIC_VP_OFFSET;
	struct irqchip *chip = &gic->vp.chip;
	struct irqdesc *parent;

	chip->name	= "mips-gic-vp";
	chip->irq_nbr	= GIC_NBR_VP_IRQS;
	chip->iobase	= base;

	chip->default_handler = irq_handle_percpu;
	chip->mask	= gic_vp_irq_mask;
	chip->unmask	= gic_vp_irq_unmask;

	parent = irq_get_desc("cpuintc", GIC_PIN_LOCAL);
	irqchip_chain(parent, gic_vp_irq_handler, chip);

	gic_init_local(gic, 0);
}


static void __init gic_init(void)
{
	struct gic_intctrl *gic = &gic_intctrl;
	void __iomem *gic_base;

	gic_base = ioremap(GIC_BASE, GIC_SIZE);

	gic_sh_init(gic, gic_base);
	gic_vp_init(gic, gic_base);
}
board_irq_initcall(gic_init);

////////////////////////////////////////////////////////////////////////////////
// SMP

#define	ipi_irq(cpu, ipi)	(GIC_OFF_IP_IRQS + (cpu) * 2 + (ipi))
#define	irq_ipi(ipi)		((ipi) % 2)

void __smp_ipi_send(uint cpu, uint ipi)
{
	struct gic_intctrl *gic = &gic_intctrl;
	void __iomem *base = gic->sh.chip.iobase;
	uint irq = ipi_irq(cpu, ipi);

	iowrite32(base + GIC_SH_WEDGE, GIC_SH_WEDGE_SET | irq);
}

static void gic_ipi_handler(struct irqdesc *desc)
{
	gic_sh_irq_ack(desc);
	__smp_ipi_process(irq_ipi(desc->irq));
}


static void gic_init_ipi_irq(struct gic_intctrl *gic, uint vpe, uint irq)
{
	struct irqchip *chip = &gic->sh.chip;
	void __iomem *base = chip->iobase;
	struct irqdesc *desc = &chip->descs[irq];

	desc->handler = gic_ipi_handler;
	ioset32(base + GIC_SH_EDGE(irq/32),  1 << (irq % 32));
	gic_map_irq_to_vpe(gic, irq, vpe);

	irq_unmask(desc);
}

static void gic_init_ipi(struct gic_intctrl *gic, uint cpu)
{
	uint vpe = __hwcpu(cpu);

	gic_init_ipi_irq(gic, vpe, ipi_irq(cpu, 0));
	gic_init_ipi_irq(gic, vpe, ipi_irq(cpu, 1));
}

static void gic_smp_init(uint cpu)
{
	struct gic_intctrl *gic = &gic_intctrl;

	gic_init_local(gic, cpu);
	irq_unmask(gic->sh.chip.parent);
	irq_unmask(gic->vp.chip.parent);

	if (cpu == 1)
		gic_init_ipi(gic, 0);

	gic_init_ipi(gic, cpu);
}
core_smp_initcall(gic_smp_init);
