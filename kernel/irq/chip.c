#include <irqdesc.h>
#include <irqchip.h>


struct irqdesc *irq_get_desc(struct irqchip *chip, unsigned int irq)
{
	struct irqdesc *desc;

	if (!chip)
		chip = &mach_irqchip;
	if (irq >= chip->irq_nbr)
		return NULL;

	desc = &chip->descs[irq];

	return desc;
}


void irqchip_init(struct irqdesc *parent, struct irqchip *chip)
{
	unsigned int i;

	for (i = 0; i < chip->irq_nbr; i++) {
		struct irqdesc *desc = &chip->descs[i];

		desc->chip = chip;
		desc->irq = i;
		lock_init(&desc->lock);
	}
}
