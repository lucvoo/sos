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

	if (desc->irq != irq)
		desc->irq = irq;

	return desc;
}
