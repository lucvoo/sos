#ifndef	_SOFTIRQ_DSR_H_
#define	_SOFTIRQ_DSR_H_

#include <softirq.h>
#include <interrupt.h>


static inline void softirq_raise_dsr(struct irqaction *action)
{
#ifdef	CONFIG_DSR
	action->dsr_count++;
	softirq_raise(SOFTIRQ_DSR);
#endif
}

#endif
