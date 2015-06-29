#include <timerdev.h>
#include <io.h>
#include <interrupt.h>
#include <init.h>
#include <irq.h>
#include <arch/memory.h>
#include <soc/tcu.h>
#include <soc/intc.h>
#include <soc/irq.h>
#include <utils.h>
#include <stringify.h>
#include <errno.h>

/*
 * The controller implement a few 16-bits timer, plus a 64-bits one (OST)
 *
 * The OST as a continuous counter.
 * One of the other timers is used as a one-shot timer.
 * Those other counters can't be used as (HW) periodic timers.
 */

struct jztimer {
	struct timerdev	td;
	unsigned int	channel;
};

/**********************************************************************/
static int ostimer_isr(struct irqdesc *desc, void *data)
{
	struct timerdev *td = data;
	unsigned int chan = TCU_CH_OST;

	iowrite32(td->base + TCU_TFCR, 1 << chan);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

static int ostimer_dsr(struct irqdesc *desc, unsigned int count, void *data)
{
	struct timerdev *td = data;

	td->handler(td);
	return 0;
}

static unsigned long ostimer_now(struct timerdev *td)
{
	unsigned long now = ioread32(td->base + OST_CNTL);

	// FIXME: 64-bit!

	return now;
}

static int ostimer_next_abs(struct timerdev *td, unsigned long val)
{
	unsigned int chan = TCU_CH_OST;

	iowrite32(td->base + OST_CMP, val);
	iowrite32(td->base + TCU_TFCR, 1 << chan);
	iowrite32(td->base + TCU_TMCR, 1 << chan);
	return 0;
}

static struct timerdev ostimer = {
	.name = "ostimer",
	.now = ostimer_now,
	.next_abs = ostimer_next_abs,
};
static struct irqaction ostimer_irq;

static int __init ostimer_init(void __iomem *base)
{
	struct irqdesc *desc;
	int rc;

	desc = irq_get_desc("intc", INTC_IRQ_TCU0);
	if (!desc) {
		return -EINVAL;
	}

	ostimer.base = base;

	irq_create(&ostimer_irq, ostimer_isr, ostimer_dsr, &ostimer, 0);
	irq_attach(desc, &ostimer_irq);

	iowrite32(base + OST_CNTL, 0);
	iowrite32(base + OST_CNTH, 0);
	iowrite32(base + OST_CMP, 0);
	iowrite32(base + OST_CSR, TCU_CSR_RTC_EN | TCU_CSR_DIV_1 | TCU_CSR_CNT_MD);

	iowrite32(base + TCU_TESR, 1 << TCU_CH_OST);
	irq_unmask(desc);

	rc = timerdev_register(&ostimer);
	return rc;
}


/**********************************************************************/
static int timer_isr(struct irqdesc * desc, void* data)
{
	struct jztimer *jzt = data;
	struct timerdev *td = &jzt->td;

	iowrite32(td->base + TCU_TFCR, 1 << jzt->channel);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

static int timer_dsr(struct irqdesc *desc, unsigned int count, void* data)
{
	struct jztimer *jzt = data;
	struct timerdev *td = &jzt->td;

	td->handler(td);
	return 0;
}

static int timer_next_abs(struct timerdev *td, unsigned long val)
{
	struct jztimer *jzt = container_of(td, struct jztimer, td);
	unsigned int channel = jzt->channel;

	iowrite32(td->base + TCU_TMSR, 1 << channel);
	iowrite32(td->base + TCU_TDFR(channel), val);
	iowrite32(td->base + TCU_TCNT(channel), 0);
	iowrite32(td->base + TCU_TESR, 1 << channel);
	iowrite32(td->base + TCU_TMCR, 1 << channel);

	return 0;
}

static struct jztimer timer0 = {
	.td = {
		.name = "timer0",
		.freq = 32*1024,
		.next_abs = timer_next_abs,
	},
};
static struct irqaction timer0_irq;

static int __init timer0_init(void __iomem *base)
{
	struct irqdesc *desc;
	int channel = 0;
	int rc;

	desc = irq_get_desc("intc", INTC_IRQ_TCU2);
	if (!desc) {
		return -EINVAL;
	}

	timer0.td.base = base;
	timer0.channel = channel;

	irq_create(&timer0_irq, timer_isr, timer_dsr, &timer0, 0);
	irq_attach(desc, &timer0_irq);

	iowrite32(base + TCU_TDHR(channel), 0xffff);	// not used, must not be reached
	iowrite32(base + TCU_TCSR(channel), TCU_CSR_RTC_EN | TCU_CSR_DIV_1);
	irq_unmask(desc);

	rc = timerdev_register(&timer0.td);
	return rc;
}

static void __init timer_init(void)
{
	void __iomem *base;

	base = ioremap(TCU_BASE, TCU_SIZE);
	if (!base) {
		return;
	}

	ostimer_init(base);
	// FIXME: unused for now: timer0_init(base);
}
board_initcall(timer_init);
