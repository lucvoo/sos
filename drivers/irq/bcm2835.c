#include <irqchip.h>
#include <irqdesc.h>
#include <io.h>
#include <soc/irqs.h>
#include <bitops/findbit.h>
#include <init.h>
#include <bits.h>


static void bcm283x_irq_unmask(struct irqdesc *desc)
{
	void __iomem *iobase = desc->chip->iobase;
	uint bank = desc->irq / 32;
	uint irq = desc->irq % 32;

	iowrite32(iobase + 0x10 + bank * 4, 1 << irq);
}

static void bcm283x_irq_mask(struct irqdesc *desc)
{
	void __iomem *iobase = desc->chip->iobase;
	uint bank = desc->irq / 32;
	uint irq = desc->irq % 32;

	iowrite32(iobase + 0x1c + bank * 4, 1 << irq);
}

////////////////////////////////////////////////////////////////////////////////
#define	NBR_IRQS		(2 * 32 + 8)

static struct bcm283x_intctrl {
	struct irqchip	chip;
	struct irqdesc	descs[NBR_IRQS];
} bcm283x_intctrl;

static void bcm283x_irq_init(void)
{
	void __iomem *iobase = ioremap(0x2000b200, 0x200);	// FIXME
	struct irqchip *chip = &bcm283x_intctrl.chip;

	chip->iobase	= iobase;
	chip->name	= "intc";
	chip->irq_nbr	= NBR_IRQS;

	chip->mask	= bcm283x_irq_mask;
	chip->unmask	= bcm283x_irq_unmask;

	irqchip_init(NULL, chip);
}
board_irq_initcall(bcm283x_irq_init);

////////////////////////////////////////////////////////////////////////////////
#include <interrupt.h>
struct eframe;

#define BANK0_MASK		MASK(0, 8)
#define BANK1_MASK		((1 << 8) | MASK(10, 5))
#define BANK2_MASK		((1 << 9) | MASK(15, 6))
#define BANK0_VALID_MASK	(BANK0_MASK | BANK1_MASK | BANK2_MASK)

static void bcm283x_handle_irq(struct eframe *regs)
{
	struct irqchip *chip = &bcm283x_intctrl.chip;
	void __iomem *iobase = chip->iobase;

	while (1) {
		u32 stat = ioread32(iobase + 0x00);
		uint off, bit, irq;

		if (stat & BANK0_MASK) {
			off = 64;
		} else if (stat & BANK1_MASK) {
			stat = ioread32(iobase + 0x04);
			off = 0;
		} else if (stat & BANK2_MASK) {
			stat = ioread32(iobase + 0x08);
			off = 32;
		} else
			break;

		bit = bitop_ctz(stat);
		irq = off + bit;
		__irq_handler(&chip->descs[irq], regs);
	}
}

#include <symbols.h>
strong_alias(bcm283x_handle_irq, mach_irq_handler);
