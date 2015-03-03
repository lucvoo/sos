#ifndef	_IRQ_H_
#define	_IRQ_H_

#include <irqchip.h>


static inline void irq_ack(int irq)
{
	if (mach_irqchip.ack)
		mach_irqchip.ack(irq);
}

static inline void irq_mask(int irq)
{
	if (mach_irqchip.mask)
		mach_irqchip.mask(irq);
}

static inline void irq_mask_ack(int irq)
{
	if (mach_irqchip.mask_ack)
		mach_irqchip.mask_ack(irq);
	else {
		irq_mask(irq);
		irq_ack(irq);
	}
}

static inline void irq_unmask(int irq)
{
	if (mach_irqchip.unmask)
		mach_irqchip.unmask(irq);
}

#endif
