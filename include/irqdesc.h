#ifndef	_IRQDESC_H_
#define	_IRQDESC_H_

#include <lock.h>


struct irqaction;

struct irqdesc {
	struct irqaction*	action;
	unsigned int		irq;
	struct lock		lock;
};

#endif
