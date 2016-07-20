#ifndef	_ARCH_IRQ_STAT_H_
#define	_ARCH_IRQ_STAT_H_

struct irq_stat {
	unsigned int softirq_pending;
	unsigned int local_timer_irqs;
};

#include <irq_stat.h>	/* Standard mappings for irq_cpustat_t above */

#endif
