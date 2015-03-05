#include <irqdesc.h>
#include <irqchip.h>


static struct irqchip *irqchip_root;


struct irqdesc *irq_get_desc(struct irqchip *chip, unsigned int irq)
{
	struct irqdesc *desc;

	if (!chip)
		chip = irqchip_root;
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
