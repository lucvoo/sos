#ifndef	_DSR_H_
#define	_DSR_H_

struct irqaction;

void __softirq_raise_dsr(struct irqaction *action);

static inline void softirq_raise_dsr(struct irqaction *action)
{
#ifdef	CONFIG_DSR
	__softirq_raise_dsr(action);
#endif
}

#endif
