#include <timerdev.h>
#include <interrupt.h>
#include <arch/copro.h>
#include <init.h>
#include <irq.h>
#include <errno.h>


struct mips_timer {
	struct timerdev		td;
	struct irqaction	irq;
	struct irqdesc		*desc;
};

/**********************************************************************/
static int mips_timer_isr(struct irqdesc *desc, void* data)
{
	u32 comp;

	if (!(c0_getval(c0_cause) & CAUSE_TI))
		return IRQ_NONE;

	comp = c0_getval(c0_compare);
	c0_setval(c0_compare, comp);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

static int mips_timer_dsr(struct irqdesc *desc, unsigned int count, void* data)
{
	struct mips_timer *mt = data;
	struct timerdev *td = &mt->td;

#ifdef CONFIG_SMP
#error "FIXME: need support for per-cpu timers"
#endif

	td->handler(td);
	return 0;
}

static unsigned long mips_timer_now(struct timerdev *td)
{
	return c0_getval(c0_count);
}

static int mips_timer_next_abs(struct timerdev *td, unsigned long next)
{
	c0_setval(c0_compare, next);

	if ((int)(c0_getval(c0_count) - next) >= 0)
		return -EINVAL;

	return 0;
}

static int mips_timer_next_rel(struct timerdev *td, unsigned long val)
{
	u32 curr;
	u32 next;

	curr = c0_getval(c0_count);
	next = curr + val;
	c0_setval(c0_compare, next);

	if ((int)(c0_getval(c0_count) - next) >= 0)
		return -EINVAL;

	return 0;
}


static int __init mips_timer_probe(struct mips_timer *mt)
{
	struct irqdesc *desc;
	int irq;
	int rc;

	// FIXME: handle external interrupt controller

	irq = INTCTL_IPTI(c0_getval(c0_intctl));

	desc = irq_get_desc("cpuintc", irq);
	if (!desc)
		return -EINVAL;

#ifdef CONFIG_SMP
#error "FIXME: need support for per-cpu timers"
#endif

	mt->desc = desc;
	mt->td.name = "arch-timer";
	mt->td.next_abs = mips_timer_next_abs;
	mt->td.next_rel = mips_timer_next_rel;
	mt->td.now = mips_timer_now;

	irq_create(&mt->irq, mips_timer_isr, mips_timer_dsr, mt, 0);
	irq_attach(desc, &mt->irq);

	c0_setval(c0_count, 0);
	c0_setval(c0_compare, -1);
	irq_unmask(desc);

	rc = timerdev_register(&mt->td);
	return rc;
}

static struct mips_timer mips_timer;
static void __init mips_timer_init(void)
{
	mips_timer_probe(&mips_timer);
}
board_initcall(mips_timer_init);
