#ifndef	_IRQ_H_
#define	_IRQ_H_

#include <arch/irq.h>
#include <lock.h>


struct irqaction;

struct irqdesc {
	struct irqaction*	action;
	struct lock		lock;
};

#endif
