#include <irqchip.h>
#include <soc/gpio.h>
#include <soc/baseaddr.h>
#include <io.h>
#include <irq.h>
#include <init.h>


static void jz4780_gpio_irq_ack(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + GPIO_FLGC(0), 1 << irq);
}

static void jz4780_gpio_irq_mask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + GPIO_MSKS(0), 1 << irq);
}

static void jz4780_gpio_irq_mask_ack(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + GPIO_MSKS(0), 1 << irq);
	iowrite32(desc->chip->iobase + GPIO_FLGC(0), 1 << irq);
}

static void jz4780_gpio_irq_unmask(struct irqdesc *desc)
{
	unsigned int irq = desc->irq;

	iowrite32(desc->chip->iobase + GPIO_MSKC(0), 1 << irq);
}


struct jz4780_gpio {
	struct irqchip	irqchip;
	struct irqdesc	irqs[32];
} jz4780_gpios[6];


static void jz4780_gpio_irq_handler(struct irqdesc *desc)
{
	int parent_irqn = desc->irq;
	int port = 17 - parent_irqn;
	struct irqchip *chip = &jz4780_gpios[port].irqchip;
	unsigned int pend;
	unsigned int mask;
	unsigned int irq;

	pend = ioread32(chip->iobase + GPIO_FLG(0));
	mask = ioread32(chip->iobase + GPIO_MSK(0));

	pend = pend & ~mask;
	for (irq = 32; pend && irq--;) {
		unsigned int mask = 1 << irq;

		if (pend & mask) {
			irq_handle_desc(&chip->descs[irq]);
			pend ^= mask;
		}
	}
}


static void jz4780_gpio_init_one(struct jz4780_gpio *chip, int port, const char *name)
{
	int parent_irqn = 17 - port;
	struct irqchip *irqchip;
	struct irqdesc *parent;
	void __iomem *iobase;

	parent = irq_get_desc("intc", parent_irqn);
	parent->handler = jz4780_gpio_irq_handler;

	iobase = ioremap(GPIO_BASE + 0x100 * port, 0x100);

	// IRQ part
	irqchip = &chip->irqchip;
	irqchip->name		= name;
	irqchip->irq_nbr	= 32;
	irqchip->iobase		= iobase;

	irqchip->ack		= jz4780_gpio_irq_ack;
	irqchip->mask		= jz4780_gpio_irq_mask;
	irqchip->mask_ack	= jz4780_gpio_irq_mask_ack;
	irqchip->unmask		= jz4780_gpio_irq_unmask;

	// mask all interrupt (and only the ones configured as such)
	iowrite32(iobase + GPIO_MSKS(0), ioread32(iobase + GPIO_INT(0)));

	irqchip_init(parent, irqchip);
	irq_unmask(parent);
}

static void jz4780_gpio_init(void)
{
	jz4780_gpio_init_one(&jz4780_gpios[0], 0, "gpa");
	jz4780_gpio_init_one(&jz4780_gpios[1], 1, "gpb");
	jz4780_gpio_init_one(&jz4780_gpios[2], 2, "gpc");
	jz4780_gpio_init_one(&jz4780_gpios[3], 3, "gpd");
	jz4780_gpio_init_one(&jz4780_gpios[4], 4, "gpe");
	jz4780_gpio_init_one(&jz4780_gpios[5], 5, "gpf");
}
board_core_initcall(jz4780_gpio_init);
