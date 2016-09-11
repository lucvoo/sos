#include <interrupt.h>
#include <irq.h>
#include <irqdesc.h>
#include <irq_stat.h>
#include <irqflags.h>
#include <lock.h>
#include "dsr.h"


static struct irqdesc bad_irqdesc = {
};


static int handle_action_unlocked(struct irqdesc *desc, struct irqaction *action)
{
	int ret;

	ret = action->isr_handler(desc, action->data);
	// FIXME: check if handled

	if (ret & IRQ_CALL_DSR) {
		softirq_raise_dsr(action);
	}

	return ret;
}

static int handle_action_locked(struct irqdesc *desc, struct irqaction *action)
{
	int ret;

	lock_rel(&desc->lock);
	ret = handle_action_unlocked(desc, action);
	lock_acq(&desc->lock);

	return ret;
}

/*
 * IRQ flow handler for 'level' IRQ
 *	->mask_ack()
 *	handle IRQ
 *	->unmask()
 */
void irq_handle_level(struct irqdesc *desc)
{
	struct irqaction *action;
	int ret __unused;

	lock_acq(&desc->lock);
	irq_mask_ack(desc);

	action = desc->action;
	if (unlikely(!action))		// leave it unmasked!
		goto out_unlock;

	ret = handle_action_locked(desc, action);

	irq_unmask(desc);

out_unlock:
	lock_rel(&desc->lock);
}

/*
 * IRQ flow handler for smart controller
 *	handle IRQ
 *	->eoi()
 */
void irq_handle_eoi(struct irqdesc *desc)
{
	struct irqaction *action;
	int ret __unused;

	lock_acq(&desc->lock);

	action = desc->action;
	if (unlikely(!action)) {
		desc->chip->mask(desc);	// leave it masked!
		goto out_unlock;
	}

	ret = handle_action_locked(desc, action);
	desc->chip->eoi(desc);

out_unlock:
	lock_rel(&desc->lock);
}

/*
 * IRQ flow handler for simple & SW decoded IRQ:
 *	handle IRQ
 *
 * Used when everything is handled in HW
 * or by the parent of nested controllers.
 */
void irq_handle_simple(struct irqdesc *desc)
{
	struct irqaction *action;
	int ret __unused;

	lock_acq(&desc->lock);

	action = desc->action;
	if (unlikely(!action)) {
		goto out_unlock;
	}

	ret = handle_action_locked(desc, action);

out_unlock:
	lock_rel(&desc->lock);
}

/*
 * IRQ flow handler for local IRQ handling
 *	handle IRQ
 *
 * Used for percpu IRQs
 */
void irq_handle_percpu(struct irqdesc *desc)
{
	struct irqchip *chip = desc->chip;
	struct irqaction *action;
	int ret __unused;

	if (chip->ack)
		chip->ack(desc);

	action = desc->action;		// assume action is never NULL
	ret = handle_action_unlocked(desc, action);

	if (chip->eoi)
		chip->eoi(desc);
}


void irq_handle_desc(struct irqdesc *desc)
{
	desc->handler(desc);
}

// Normal entry for the main irqchip
void __irq_handler(struct irqdesc *desc, struct eframe *regs)
{

	(void) regs;

	if (!desc)
		desc = &bad_irqdesc;

	desc->handler(desc);

	if (softirq_pending())
		__do_softirq();
}

// Entry point for the main irqchip when written in asm (like ARMv4)
void __do_IRQ(unsigned int irq, struct eframe *regs)
{
	struct irqdesc *desc = irq_get_desc(NULL, irq);

	return __irq_handler(desc, regs);
}
