#ifndef	_ARCH_IRQ_STAT_H_
#define	_ARCH_IRQ_STAT_H_

struct irq_stat {
#ifdef	CONFIG_SOFTIRQ
	unsigned int softirq_pending;
#endif
};

#endif
