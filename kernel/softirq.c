#include <interrupt.h>
#include <irqflags.h>
#include <irq_stat.h>
#include <softirq.h>


struct irq_stat irq_stat;

static struct softirq_action softirqs[SOFTIRQ_MAX];


void __do_softirq(void)
{
	struct softirq_action *h;
	unsigned int pending;

	pending = softirq_pending();

	do {
		softirq_set_pending(0);
		__local_irq_ena();
		h = softirqs;

		do {
			if (pending & 1)
				h->action(h);
			h++;
			pending >>= 1;
		} while (pending);

		__local_irq_dis();

		pending = softirq_pending();
	} while (pending);
}
