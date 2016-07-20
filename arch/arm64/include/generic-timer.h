#ifndef	_ARCH_GENERIC_TIMER_H_
#define	_ARCH_GENERIC_TIMER_H_

#include <hw/arm-generic-timer.h>
#include <arch/msr.h>

static inline u64 arch_timer_get_counter(void)
{
	return msr_getval(cntvct_el0);
}

static inline u32 arch_timer_get_ctrl(void)
{
	return msr_getval(cntv_ctl_el0);
}

static inline void arch_timer_set_ctrl(u32 val)
{
	return msr_setval(cntv_ctl_el0, val);
}

static inline void arch_timer_set_tval(u32 val)
{
	return msr_setval(cntv_tval_el0, val);
}


static inline u32 arch_timer_get_kctl(void)
{
	return msr_getval(cntkctl_el1);
}

static inline void arch_timer_set_kctl(u32 val)
{
	return msr_setval(cntkctl_el1, val);
}

#endif
