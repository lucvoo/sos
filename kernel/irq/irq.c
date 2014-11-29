#include <interrupt.h>
#include <irq.h>
#include <irq_stat.h>
#include <irqflags.h>
#include <lock.h>
#include <softirq.h>
#include <init.h>
#include <errno.h>


static struct irqdesc irq_descs[NR_IRQS];

static struct irqdesc bad_irqdesc = {
};


static int handle_IRQ_event(unsigned int irq, struct irqaction *action)
{
	int ret;

	ret = action->isr_handler(irq, action->data);
	// FIXME: check if handled

#ifdef	CONFIG_DSR
	if (ret & IRQ_CALL_DSR) {
		action->dsr_count++;
		softirq_raise(SOFTIRQ_DSR);
	}
#endif

	return ret;
}

static void handle_level_irq(unsigned int irq, struct irqdesc *desc)
{
	struct irqaction *action;
	int ret;

	lock_acq(&desc->lock);
	irq_mask_ack(irq);

	action = desc->action;
	if (unlikely(!action))		// leave it unmasked!
		goto out_unlock;

	lock_rel(&desc->lock);

	ret = handle_IRQ_event(irq, action);

	lock_acq(&desc->lock);

	irq_unmask(irq);

out_unlock:
	lock_rel(&desc->lock);
}

void __do_IRQ(unsigned int irq, struct eframe *regs)
{
	struct irqdesc *desc = irq_descs + irq;

	(void) regs;

	if (irq >= NR_IRQS)
		desc = &bad_irqdesc;

	handle_level_irq(irq, desc);

	if (softirq_pending())
		__do_softirq();
}


void irq_create(struct irqaction* action, isr_handler_t isr_handler, dsr_handler_t dsr_handler, void* data, unsigned long flags)
{
	action->isr_handler = isr_handler;
#ifdef	CONFIG_DSR
	action->dsr_handler = dsr_handler;
#endif
	action->data        = data;
	action->flags       = flags;
}

int irq_attach(struct irqaction* action, int irq)
{
	struct irqdesc* desc = &irq_descs[irq];

	if (irq >= NR_IRQS)
		return -EINVAL;

	lock_acq_irq(&desc->lock);
	desc->action = action;
	lock_rel_irq(&desc->lock);

	return 0;
}

#ifdef	CONFIG_DSR
static void softirq_dsr(struct softirq_action* action)
{
	int irq;

	for (irq = 0; irq < NR_IRQS; irq++) {
		struct irqaction *irqaction = irq_descs[irq].action;
		unsigned long count;

		if (!irqaction || !irqaction->dsr_count)
			continue;
		irq_mask(irq);
		count = irqaction->dsr_count;
		irqaction->dsr_count = 0;
		irq_unmask(irq);
		irqaction->dsr_handler(irq, count, irqaction->data);
	}
}

static void init_softirq_dsr(void)
{
	softirq_register(SOFTIRQ_DSR, softirq_dsr, 0);
}
pure_initcall(init_softirq_dsr);
#endif
