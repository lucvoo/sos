#ifndef	_IRQ_STAT_H_
#define	_IRQ_STAT_H_

#include <arch/irq_stat.h>

extern struct irq_stat irq_stat;

#define	softirq_pending()	irq_stat.softirq_pending
#define	softirq_set_pending(x)	(softirq_pending() = (x))

#endif