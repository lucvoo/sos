#include <timerdev.h>
#include <arch/generic-timer.h>
#include <soc/generic-timer.h>
#include <arch/barrier.h>
#include <interrupt.h>
#include <errno.h>
#include <init.h>
#include <irq.h>


// FIXME: for now we only can use the virt timer
//	  we may have reasons to use another one


static inline u64 agt_counter(void)
{
	u64 val;

	isb();
	val = arch_timer_get_counter();
	return val;
}


static int agt_isr(struct irqdesc *desc, void *data)
{
	u32 ctrl;

	ctrl = arch_timer_get_ctrl();
	if (!(ctrl & CNTX_CTL_ISTATUS))
		return IRQ_NONE;

	ctrl |= CNTX_CTL_IMASK;
	arch_timer_set_ctrl(ctrl);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

// FIXME: - 32bit but HW allow 64bit
//	  - freq is quite high (20-50MHz)
//	    => rollover < 3 minutes
static ulong agt_now(struct timerdev *td)
{
	ulong now;

	now = agt_counter();
	return now;
}

static int agt_next_rel(struct timerdev *td, ulong val)
{
	u32 ctrl;

	// FIXME: overrun is quite short (3 min)

	ctrl = arch_timer_get_ctrl();
	ctrl |= CNTX_CTL_ENABLE;
	ctrl &= ~CNTX_CTL_IMASK;
	arch_timer_set_tval(val);
	arch_timer_set_ctrl(ctrl);

	return 0;
}

static int agt_stop(struct timerdev *td)
{
	u32 ctrl;

	ctrl = arch_timer_get_ctrl();
	ctrl &= ~CNTX_CTL_ENABLE;
	arch_timer_set_ctrl(ctrl);

	return 0;
}


static void agt_user_access(void)
{
	u32 ctrl = arch_timer_get_kctl();

	// only allow access to the virtual counter
	ctrl &= CNTKCTL_PRESERVE;
	ctrl |= CNTKCTL_PL0VCTEN;

	arch_timer_set_kctl(ctrl);
}

static int agt_init_local(struct timerdev *td, uint cpu)
{
	struct irqdesc *desc = irq_get_desc("gic", TIMER_IRQ);

	agt_user_access();
	irq_unmask(desc);

	return 0;
}

static void agt_stop_local(struct timerdev *td)
{
	struct irqdesc *desc = irq_get_desc("gic", TIMER_IRQ);

	irq_mask(desc);
}


static struct timerdev arm_generic_timer;
static struct irqaction agt_irq;

static int __init agt_init(void)
{
	struct timerdev *td = &arm_generic_timer;
	struct irqdesc *desc;
	int rc = EINVAL;

	desc = irq_get_desc("gic", TIMER_IRQ);
	if (!desc)
		goto err_irqdesc;

	td->name = "generic_timer";
	td->now      = agt_now;
	td->next_rel = agt_next_rel;

	irq_create(&agt_irq, agt_isr, timerdev_dsr, td, 0);
	irq_attach(desc, &agt_irq);

	agt_init_local(td, 0);

	rc = timerdev_register(td);
	return rc;

err_irqdesc:
	return rc;
}

static void __init arm_generic_timer_init(void)
{
	agt_init();
}
board_initcall(arm_generic_timer_init);

#include <smp/initcall.h>
static void arm_generic_timer_smp_init(uint cpu)
{
	agt_init_local(&arm_generic_timer, cpu);
}
core_smp_initcall(arm_generic_timer_smp_init);
