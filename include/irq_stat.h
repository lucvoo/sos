#ifndef	_IRQ_STAT_H_
#define	_IRQ_STAT_H_

#include <arch/irq_stat.h>

extern struct irq_stat irq_stat;

#ifdef	CONFIG_SOFTIRQ
#define	softirq_pending()	irq_stat.softirq_pending
#define	softirq_set_pending(x)	(softirq_pending() = (x))

#define softirq_raise(nr) do { softirq_pending() |= (1UL << (nr)); } while (0)
#else
#define	softirq_pending()	0
#define	__do_softirq()		do ; while (0)
#endif

#endif
