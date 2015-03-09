#include <bitops/findbit.h>
#include <irqchip.h>
#include <soc/intc.h>
#include <soc/irq.h>
#include <io.h>
#include <irq.h>
#include <init.h>


#define	NBR_IRQS	IRQ_INTC_NBR

struct jz4780_intc_intctrl {
	struct irqchip	chip;
	struct irqdesc	descs[NBR_IRQS];
} jz4780_intc_intctrl;


static void jz4780_intc_irq_mask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + INTC_ICMSR(irq / 32), 1 << (irq % 32));
}

static void jz4780_intc_irq_unmask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + INTC_ICMCR(irq / 32), 1 << (irq % 32));
}


static void jz4780_intc_irq_handler(struct irqdesc *desc)
{
	struct irqchip *chip = &jz4780_intc_intctrl.chip;
	unsigned int n = 0;
	unsigned int irqs;

again:
	irqs = ioread32(chip->iobase + INTC_ICPR(0));
	if (irqs) {
		do {
			unsigned int irq;

			irq = bitop_ctz(irqs);
			irqs ^= 1 << irq;
			irq_handle_desc(&chip->descs[irq +  0]);
		} while (irqs);

		n++;
		goto again;
	}

	irqs = ioread32(chip->iobase + INTC_ICPR(1));
	if (irqs) {
		do {
			unsigned int irq;

			irq = bitop_ctz(irqs);
			irqs ^= 1 << irq;
			irq_handle_desc(&chip->descs[irq + 32]);
		} while (irqs);

		n++;
		goto again;
	}

	if (n == 0)
		return;	// FIXME: spurious interrupt
}


static void jz4780_intc_irq_init(void)
{
	struct irqchip *chip = &jz4780_intc_intctrl.chip;
	struct irqdesc *parent;
	void __iomem *iobase;

	parent = irq_get_desc(NULL, IRQ_CPU_INTC);
	parent->handler = jz4780_intc_irq_handler;

	iobase = ioremap(INTC_BASE, INTC_SIZE);

	chip->name	= "intc";
	chip->irq_nbr	= NBR_IRQS;
	chip->iobase	= iobase;

	chip->mask	= jz4780_intc_irq_mask;
	chip->mask_ack	= jz4780_intc_irq_mask;
	chip->unmask	= jz4780_intc_irq_unmask;

	// Mask all the interrupts
	iowrite32(chip->iobase + INTC_ICMSR(0), 0xffffffff);
	iowrite32(chip->iobase + INTC_ICMSR(1), 0xffffffff);

	irqchip_init(parent, chip);
	irq_unmask(parent);
}
board_irq_initcall(jz4780_intc_irq_init);
