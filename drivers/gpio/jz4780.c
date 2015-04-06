#include "gpiochip.h"
#include <irqchip.h>
#include <soc/gpio.h>
#include <soc/baseaddr.h>
#include <io.h>
#include <irq.h>
#include <init.h>


static void jz4780_gpio_input(struct gpio *gpio)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	iowrite32(iobase + GPIO_PAT1S(0), 1 << n);
}

static void jz4780_gpio_set_value(struct gpio *gpio, int val)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	unsigned long reg;
	int n = gpio->idx;

	if (val)
		reg = GPIO_PAT1S(0);
	else
		reg = GPIO_PAT1C(0);
	iowrite32(iobase + reg, 1 << n);
}

static void jz4780_gpio_output(struct gpio *gpio, int val)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	jz4780_gpio_set_value(gpio, val);
	iowrite32(iobase + GPIO_PAT1C(0), 1 << n);
}

static int  jz4780_gpio_get_value(struct gpio *gpio)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	return (ioread32(iobase + GPIO_PIN(0)) >> n) & 0x1;
}

static void jz4780_gpio_set_irq(struct gpio *gpio, enum gpio_trigger_type type)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	unsigned long pat1, pat0;
	int n = gpio->idx;

	switch (type) {
	case GPIO_TRIG_LEVEL_LOW:
		pat1 = GPIO_PAT1C(0);
		pat0 = GPIO_PAT0C(0);
		break;
	case GPIO_TRIG_LEVEL_HIGH:
		pat1 = GPIO_PAT1C(0);
		pat0 = GPIO_PAT0S(0);
		break;
	case GPIO_TRIG_EDGE_FALL:
		pat1 = GPIO_PAT1S(0);
		pat0 = GPIO_PAT0C(0);
		break;
	case GPIO_TRIG_EDGE_RISE:
		pat1 = GPIO_PAT1S(0);
		pat0 = GPIO_PAT0S(0);
		break;
	default:
		// FIXME
		return;
	}

	iowrite32(iobase + GPIO_MSKS(0), 1 << n);
	iowrite32(iobase + GPIO_INTS(0), 1 << n);
	iowrite32(iobase + pat1, 1 << n);
	iowrite32(iobase + pat0, 1 << n);
}

static void jz4780_gpio_alt_func(struct gpio *gpio, int func)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	switch (func) {
	case -1: // GPIO input
		iowrite32(iobase + GPIO_INTC(0), 1 << n);
		iowrite32(iobase + GPIO_MSKC(0), 1 << n);
		iowrite32(iobase + GPIO_PAT1S(0), 1 << n);
		break;
	case 0:
		iowrite32(iobase + GPIO_INTC(0), 1 << n);
		iowrite32(iobase + GPIO_MSKC(0), 1 << n);
		iowrite32(iobase + GPIO_PAT1C(0), 1 << n);
		iowrite32(iobase + GPIO_PAT0C(0), 1 << n);
		break;
	case 1:
		iowrite32(iobase + GPIO_INTC(0), 1 << n);
		iowrite32(iobase + GPIO_MSKC(0), 1 << n);
		iowrite32(iobase + GPIO_PAT1C(0), 1 << n);
		iowrite32(iobase + GPIO_PAT0S(0), 1 << n);
		break;
	case 2:
		iowrite32(iobase + GPIO_INTC(0), 1 << n);
		iowrite32(iobase + GPIO_MSKC(0), 1 << n);
		iowrite32(iobase + GPIO_PAT1S(0), 1 << n);
		iowrite32(iobase + GPIO_PAT0C(0), 1 << n);
		break;
	case 3:
		iowrite32(iobase + GPIO_INTC(0), 1 << n);
		iowrite32(iobase + GPIO_MSKC(0), 1 << n);
		iowrite32(iobase + GPIO_PAT1S(0), 1 << n);
		iowrite32(iobase + GPIO_PAT0S(0), 1 << n);
		break;
	default:
		// FIXME
		break;
	}
}


static const struct gpio_ops jz4780_gpio_ops = {
	.input =	jz4780_gpio_input,
	.output =	jz4780_gpio_output,
	.set_value =	jz4780_gpio_set_value,
	.get_value =	jz4780_gpio_get_value,
	.set_irq =	jz4780_gpio_set_irq,
	.alt_func =	jz4780_gpio_alt_func,
	.pull_up =	NULL,	// TODO
	.pull_on =	NULL,	// TODO
};


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
	struct gpiochip	gpiochip;
	struct gpio	gpios[32];
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
	struct gpiochip *gpiochip;
	struct irqchip *irqchip;
	struct irqdesc *parent;
	void __iomem *iobase;

	parent = irq_get_desc("intc", parent_irqn);
	parent->handler = jz4780_gpio_irq_handler;

	iobase = ioremap(GPIO_BASE + 0x100 * port, 0x100);

	// pure GPIO part
	gpiochip = &chip->gpiochip;
	gpiochip->name = name;
	gpiochip->nbr = 32;
	gpiochip->iobase = iobase;
	gpiochip->ops = &jz4780_gpio_ops;
	gpiochip->gpios = chip->gpios;
	gpiochip->irqs = chip->irqs;
	gpiochip_register(gpiochip);

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
