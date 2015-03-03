#ifndef	_SOFTIRQ_DSR_H_
#define	_SOFTIRQ_DSR_H_

#include <softirq.h>
#include <interrupt.h>

#ifdef	CONFIG_DSR
#include <lock.h>

struct irqaction*	dsr_list;
struct lock		dsr_lock = LOCK_INIT;


static inline void softirq_raise_dsr(struct irqaction *action)
{
	struct irqdesc *desc = action->desc;
	unsigned long flags;

	flags = lock_acq_save(&desc->lock);
	if (++action->dsr_count == 1) {
		action->dsr_next = dsr_list;
		dsr_list = action;
		softirq_raise(SOFTIRQ_DSR);
	}
	lock_rel_rest(&desc->lock, flags);
}
#else
static inline void softirq_raise_dsr(struct irqaction *action)
{
}
#endif

#endif
