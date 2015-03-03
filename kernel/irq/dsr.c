#include "dsr.h"
#include <softirq.h>
#include <irq_stat.h>
#include <irqdesc.h>
#include <interrupt.h>
#include <lock.h>
#include <init.h>

static struct irqaction*	dsr_list;
static struct lock		dsr_lock = LOCK_INIT;


void __softirq_raise_dsr(struct irqaction *action)
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

static void softirq_dsr(struct softirq_action* action)
{
	struct irqaction *curr;

	lock_acq_irq(&dsr_lock);
	while ((curr = dsr_list)) {
		unsigned int count;

		dsr_list = curr->dsr_next;
		count = curr->dsr_count;
		curr->dsr_count = 0;
		lock_rel_irq(&dsr_lock);

		curr->dsr_handler(curr->desc, count, curr->data);

		lock_acq_irq(&dsr_lock);
	}
	lock_rel_irq(&dsr_lock);
}

static void init_softirq_dsr(void)
{
	softirq_register(SOFTIRQ_DSR, softirq_dsr, NULL);
}
pure_initcall(init_softirq_dsr);
