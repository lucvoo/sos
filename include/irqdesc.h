#ifndef	_IRQDESC_H_
#define	_IRQDESC_H_

#include <lock.h>


struct irqaction;
struct irqchip;

struct irqdesc {
	struct irqaction*	action;
	unsigned int		irq;		// relative to the chip
	struct lock		lock;
};

struct irqdesc *irq_get_desc(struct irqchip *chip, unsigned int irq);

#endif
