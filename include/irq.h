#ifndef	_IRQ_H_
#define	_IRQ_H_

#include <irqchip.h>


static inline void irq_ack(struct irqdesc *desc)
{
	if (mach_irqchip.ack)
		mach_irqchip.ack(desc);
}

static inline void irq_mask(struct irqdesc *desc)
{
	if (mach_irqchip.mask)
		mach_irqchip.mask(desc);
}

static inline void irq_mask_ack(struct irqdesc *desc)
{
	if (mach_irqchip.mask_ack)
		mach_irqchip.mask_ack(desc);
	else {
		irq_mask(desc);
		irq_ack(desc);
	}
}

static inline void irq_unmask(struct irqdesc *desc)
{
	if (mach_irqchip.unmask)
		mach_irqchip.unmask(desc);
}

#endif
