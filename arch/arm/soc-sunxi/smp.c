// SMP ops for multi-cluster SoCs

#include <smp.h>
//#include <arch/asm-offsets.h>
#include <arch/memory.h>
#include <soc/cpucfg.h>
#include <soc/prcm.h>
#include <soc/smp.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <thread.h>
#include <delay.h>
#include <io.h>


static int sunxi_smp_init(void)
{
	void __iomem *cpucfg_base = ioremap(CPUCFG_BASE, CPUCFG_SIZE);
void __iomem *prcm_base = ioremap(PRCM_BASE, PRCM_SIZE);

	iowrite32(cpucfg_base + CPUCFG_PRIVATE0, virt_to_phys(__smp_entry));

printf("smp entry: %lx @ %p\n", virt_to_phys(__smp_entry), cpucfg_base + CPUCFG_PRIVATE0);
printf("prcm @ %p\n", prcm_base);
	return 0;
}

static int sunxi_enable_cpu(uint cpu)
{
	void __iomem *prcm_base = ioremap(PRCM_BASE, PRCM_SIZE);
	void __iomem *cpucfg_base = ioremap(CPUCFG_BASE, CPUCFG_SIZE);
	const uint cluster = 0;

	// 1) assert CPU core reset
	iowrite32(cpucfg_base + CPUCFG_CPUX_RST(cpu), 0);

	// 2) assert L1 cache reset
	ioclr32(cpucfg_base + CPUCFG_GENER_CTRL, 1 << cpu);
	udelay(10);

	// 3) release power clamp */
#if 0
#else
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0xFE);
	udelay(20);
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0xF8);
	udelay(10);
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0xE0);
	udelay(10);
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0x80);
	udelay(10);
	iowrite32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu), 0x00);
	udelay(20);

	while (ioread32(prcm_base + PRCM_CPU_PWR_SWITCH(cpu)))
		;
#endif

	// 4) release power-off gating */
	ioclr32(prcm_base + PRCM_CX_PWROFF_GATING(cluster), 1 << cpu);
	udelay(20);

	// 4) deassert CPU core reset */
	iowrite32(cpucfg_base + CPUCFG_CPUX_RST(cpu), 3);

	return 0;
}

static int sunxi_smp_boot_cpu(struct thread *idle, uint cpu)
{

	// FIXME: ???gic_init_cpu(cpu);

	sunxi_enable_cpu(cpu);
printf("CPU %d enabled\n", cpu);

	return 0;
}


struct smp_ops smp_ops = {
	.init	  = sunxi_smp_init,
	.boot_cpu = sunxi_smp_boot_cpu,
};
