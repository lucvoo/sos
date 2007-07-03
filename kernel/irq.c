#include <interrupt.h>
#include <irq.h>
#include <irq_stat.h>
#include <irqflags.h>
#include <lock.h>
#include <compiler.h>


static struct irqdesc irq_descs[NR_IRQS];

static struct irqdesc bad_irqdesc = {
};


static int handle_IRQ_event(unsigned int irq, struct irqaction *action)
{
	int ret;

	ret = action->isr_handler(irq, action->data);
	// FIXME: check if handled

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
}
