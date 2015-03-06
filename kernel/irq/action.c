#include <interrupt.h>
#include <irqdesc.h>
#include <lock.h>
#include <errno.h>


void irq_create(struct irqaction* action, isr_handler_t isr_handler, dsr_handler_t dsr_handler, void* data, unsigned long flags)
{
	action->isr_handler = isr_handler;
#ifdef	CONFIG_DSR
	action->dsr_handler = dsr_handler;
	action->dsr_count = 0;
	action->dsr_next = NULL;
#endif
	action->data        = data;
	action->flags       = flags;
}

int irq_attach(struct irqdesc *desc, struct irqaction* action)
{
	if (!desc)
		return -EINVAL;

	lock_acq_irq(&desc->lock);
	desc->action = action;
#ifdef	CONFIG_DSR
	action->desc = desc;
#endif
	lock_rel_irq(&desc->lock);

	return 0;
}
