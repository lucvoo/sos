#ifndef	_IRQDESC_H_
#define	_IRQDESC_H_

#include <arch/irq.h>
#include <lock.h>


struct irqaction;

struct irqdesc {
	struct irqaction*	action;
	struct lock		lock;
};

#endif
