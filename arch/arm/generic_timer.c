#include <arch/cp15.h>
#include <init.h>


#if CONFIG_CNTFRQ != 0
static void __init cntfreq_init(void)
{
	u32 pfr1;
	u32 freq;

	pfr1 = cp_read(ID_PFR1);
	if (!(pfr1 & ID_PFR1_GENERIC_TIMER))
		return;

	freq = cp_read(CNTFRQ);
	if (freq == CONFIG_CNTFRQ)
		return;

	// FIXME: need a way to know if we're in secure mode or not
#ifdef CONFIG_NON_SECURE
	// Do not try to fix it if we're not in secure mode
	// => we would crash.
	pr_err("CNTFRQ: %dHz != %dH!\n", freq, CONFIG_CNTFRQ);
#else
	pr_dbg("CNTFRQ: %dHz -> %dHz\n", freq, CONFIG_CNTFRQ);
	cp_write(CNTFRQ, CONFIG_CNTFRQ);
#endif
}
pure_initcall(cntfreq_init);
#endif
