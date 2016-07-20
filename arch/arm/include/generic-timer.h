#ifndef	_ARCH_GENERIC_TIMER_H_
#define	_ARCH_GENERIC_TIMER_H_

#include <hw/arm-generic-timer.h>
#include <arch/cp15.h>

static inline u64 arch_timer_get_counter(void)
{
	return cp_read64(CNTVCNT);
}

static inline u32 arch_timer_get_ctrl(void)
{
	return cp_read(CNTV_CTL);
}

static inline void arch_timer_set_ctrl(u32 val)
{
	return cp_write(CNTV_CTL, val);
}

static inline void arch_timer_set_tval(u32 val)
{
	return cp_write(CNTV_TVAL, val);
}


static inline u32 arch_timer_get_kctl(void)
{
	return cp_read(CNTKCTL);
}

static inline void arch_timer_set_kctl(u32 val)
{
	return cp_write(CNTKCTL, val);
}

#endif
