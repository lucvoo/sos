#include <interrupt.h>
#include <irqchip.h>
#include <arch/irq.h>
#include <arch/nvic.h>
#include <irq.h>
#include <io.h>
#include <init.h>


static void nvic_irq_mask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + NVIC_ICER(irq), 1 << (irq % 32));
}

static void nvic_irq_unmask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + NVIC_ISER(irq), 1 << (irq % 32));
}


#define	NBR_IRQS	CONFIG_NVIC_NBR_IRQ
#define	NBR_BANKS	NVIC_BANK(NBR_IRQS + 31)

static struct nvic_intctrl {
	struct irqchip	chip;
	struct irqdesc	descs[NBR_IRQS];
} nvic_intctrl;


static void nvic_irq_init(void)
{
	struct irqchip *chip = &nvic_intctrl.chip;
	void __iomem *iobase;
	int i;

	iobase = ioremap(NVIC_BASE, NVIC_SIZE);

	chip->name	= "nvic";
	chip->irq_nbr	= NBR_IRQS;
	chip->iobase	= iobase;

	chip->default_handler = irq_handle_simple;
	chip->mask	= nvic_irq_mask;
	chip->mask_ack	= nvic_irq_mask;
	chip->unmask	= nvic_irq_unmask;

	for (i = 0; i < NBR_BANKS; i++) {
		// Mask all the interrupts
		iowrite32(chip->iobase + NVIC_ICER(i * 32), ~0);
	}

	irqchip_register(chip);
}
board_irq_initcall(nvic_irq_init);


void nvic_irq_handler(unsigned int irq, struct eframe *regs);
void nvic_irq_handler(unsigned int irq, struct eframe *regs)
{
	struct irqchip *chip = &nvic_intctrl.chip;
	struct irqdesc *desc = &chip->descs[irq];

	__irq_handler(desc, regs);
}
