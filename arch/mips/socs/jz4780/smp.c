#include <arch/copro.h>
#include <arch/memory.h>
#include <io.h>
#include <soc/cgu.h>
#include <soc/regs-copro.h>
#include <thread.h>
#include <smp.h>


static struct thread idle_thread_core1;

struct thread *__smp_sec_cpu_thread;

void __smp_init(void)
{
	extern void __jz4780_smp_sec_cpu_entry(void);

	void __iomem *cgu = ioremap(CGU_BASE, CGU_SIZE);
	unsigned long entry;
	unsigned int gated;
	unsigned int lpcr;

	// initialize the thread with the default stack size
	thread_create(&idle_thread_core1, 0, NULL, NULL, NULL, 0);

	// ungate the core clock
	gated = ioread32(cgu + CGU_CLKGR1);
	gated &= ~CGU_CLKGR1_CORE1;
	iowrite32(cgu + CGU_CLKGR1, gated);

	// powerup the core
	lpcr = ioread32(cgu + CGU_LPCR);
	lpcr &= ~CGU_LPCR_SCPU_PD;
	iowrite32(cgu + CGU_LPCR, lpcr);

	// wait until powered
	while(ioread32(cgu + CGU_LPCR) & CGU_LPCR_SCPU_ST)
		;

	// setup the address of the entry point, its setup stack and un-reset the core
	entry = (unsigned long) &__jz4780_smp_sec_cpu_entry;
	__smp_sec_cpu_thread = &idle_thread_core1;
	c0_chgbits(c0_core_reim, CORE_REIM_ENTRY_MSK, entry - KSEG0_BASE + KSEG1_BASE);
	c0_chgbits(c0_core_ctrl, CORE_CTRL_SW_RST1|CORE_CTRL_RPC1, CORE_CTRL_RPC1);
}
