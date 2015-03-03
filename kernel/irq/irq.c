#include <interrupt.h>
#include <irq.h>
#include <irqdesc.h>
#include <irq_stat.h>
#include <irqflags.h>
#include <lock.h>
#include <errno.h>
#include "dsr.h"


static struct irqdesc bad_irqdesc = {
};


static int handle_IRQ_event(struct irqdesc *desc, struct irqaction *action)
{
	int ret;

	ret = action->isr_handler(desc, action->data);
	// FIXME: check if handled

	if (ret & IRQ_CALL_DSR) {
		softirq_raise_dsr(action);
	}

	return ret;
}

static void handle_level_irq(struct irqdesc *desc)
{
	struct irqaction *action;
	int ret __unused;

	lock_acq(&desc->lock);
	irq_mask_ack(desc);

	action = desc->action;
	if (unlikely(!action))		// leave it unmasked!
		goto out_unlock;

	lock_rel(&desc->lock);

	ret = handle_IRQ_event(desc, action);

	lock_acq(&desc->lock);

	irq_unmask(desc);

out_unlock:
	lock_rel(&desc->lock);
}

/**
* Normal entry for the main irqchip
*/
void __irq_handler(struct irqdesc *desc, struct eframe *regs)
{

	(void) regs;

	if (!desc)
		desc = &bad_irqdesc;

	handle_level_irq(desc);

	if (softirq_pending())
		__do_softirq();
}

/**
* Entry point for main irqchip written in asm.
*/
void __do_IRQ(unsigned int irq, struct eframe *regs)
{
	struct irqdesc *desc = irq_get_desc(NULL, irq);

	return __irq_handler(desc, regs);
}


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
