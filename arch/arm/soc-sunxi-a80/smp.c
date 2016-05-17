#include <smp.h>
#include <arch/asm-offsets.h>
#include <arch/memory.h>
#include <soc/cpucfg.h>
#include <soc/prcm.h>
#include <soc/smp.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <thread.h>
#include <delay.h>
#include <io.h>


static inline void a80_enable_cpu(uint cpu)
{
	void __iomem *prcm_base = ioremap(PRCM_BASE, PRCM_SIZE);
	void __iomem *cpucfg_base = ioremap(CPUCFG_BASE, CPUCFG_SIZE);
	uint cluster = cpu / 4;
	uint core = cpu % 4;

	// 0) set entry point
	iowrite32(prcm_base + PRCM_CPU_SOFT_ENTRY, virt_to_phys(__smp_startup));

	// 1) assert CPU power-on & core reset
	ioclr32(prcm_base + PRCM_CX_RST_CTRL(cluster), 1 << core);
	ioclr32(cpucfg_base + CPUCFG_CX_RESET(cluster), 1 << core);

	// 2) release power clamp
	// FIXME: need to go from FF, FE, FC, F8, F0 & 00
	//	  with a delay of 10us between each of these steps
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0);
	while (ioread32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu))) {
		;
	}
	mdelay(2);

	// 3) release power-off gating
	ioclr32(prcm_base + PRCM_CX_PWROFF_GATING(cluster), 1 << core);
	mdelay(1);

	// 4) deassert CPU power-on & core reset
	ioset32(cpucfg_base + CPUCFG_CX_RESET(cluster), 1 << core);
	ioset32(prcm_base + PRCM_CX_RST_CTRL(cluster), 1 << core);
}

static int a80_smp_init(void)
{
	return 0;
}

static struct thread *boot_threads[NR_CPUS];

static int a80_smp_boot_cpu(struct thread *idle, uint cpu)
{

	// FIXME: gic_init_cpu(cpu);

	// FIXME: CCI-400

	boot_threads[cpu]= idle;
	a80_enable_cpu(cpu);

	return 0;
}

void __smp_start_mach(void) __attribute__((naked));
void __smp_start_mach(void)
{
	struct thread *idle = boot_threads[__coreid()];

	set_current_thread(idle);

	asm("\n"
	"	ldr	sp, %0\n"
	"	b	__smp_start"
	:: "m" (idle->cpu_context.sp));
}

static int a80_smp_init_cpu(uint cpu)
{
	return 0;
}

struct smp_ops smp_ops = {
	.init	  = a80_smp_init,
	.boot_cpu = a80_smp_boot_cpu,
	.init_cpu = a80_smp_init_cpu,
};
