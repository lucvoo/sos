#include <irqdesc.h>
#include <irqchip.h>


static struct irqchip *irqchip_root;


struct irqdesc *irq_get_desc(const char *name, unsigned int irq)
{
	struct irqchip *chip = irqchip_root;
	struct irqdesc *desc;

	if (irq >= chip->irq_nbr)
		return NULL;

	desc = &chip->descs[irq];

	return desc;
}


void irqchip_init(struct irqdesc *parent, struct irqchip *chip)
{
	unsigned int i;

	if (!parent)
		irqchip_root = chip;

	for (i = 0; i < chip->irq_nbr; i++) {
		struct irqdesc *desc = &chip->descs[i];

		desc->chip = chip;
		desc->irq = i;
		desc->handler = chip->default_handler;
		lock_init(&desc->lock);
	}
}
