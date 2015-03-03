#include <interrupt.h>
#include <irqdesc.h>
#include <arch/irq.h>	// For NR_IRQS


static struct irqdesc irq_descs[NR_IRQS];

struct irqdesc *irq_get_desc(void *parent, unsigned int irq)
{
	struct irqdesc *desc;

	if (irq >= NR_IRQS)
		return NULL;

	desc = &irq_descs[irq];

	if (desc->irq != irq)
		desc->irq = irq;

	return desc;
}
