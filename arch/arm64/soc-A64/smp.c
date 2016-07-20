#include <arch/memory.h>
#include <soc/sys-regs.h>
#include <soc/smp.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <thread.h>
#include <delay.h>
#include <smp.h>
#include <io.h>


static void power_switch_on(void __iomem *prcm_base, uint cpu)
{
	void __iomem *pwr_clamp = prcm_base + PRCM_CPU_PWR_SWITCH(cpu);

	// step by step de-active the cpu power clamp
	iowrite32(pwr_clamp, 0xFE);
	udelay(10);

	iowrite32(pwr_clamp, 0xF8);
	udelay(10);

	iowrite32(pwr_clamp, 0xE0);
	udelay(10);

	iowrite32(pwr_clamp, 0x80);
	udelay(10);

	iowrite32(pwr_clamp, 0x00);
	udelay(10);

	while (ioread32(pwr_clamp) != 0x00)
		udelay(10);		// FIXME: timeout

	return;
}

static void power_switch_off(void __iomem *prcm_base, uint cpu)
{
	void __iomem *pwr_clamp = prcm_base + PRCM_CPU_PWR_SWITCH(cpu);

	// active the cpu power clamp
	iowrite32(pwr_clamp, 0xFF);
	udelay(30);

	while (ioread32(pwr_clamp) != 0xFF)
		udelay(10);		// FIXME: timeout

	return;
}

static void smp_secondary_entry(void __iomem *cpucfg_base, uint cpu)
{
	ulong phys_entry = virt_to_phys(__smp_startup);

	// 0) set entry point / reset vector
	//	this is like writing into RVBAR
	iowrite32(cpucfg_base + CPUCFG_RVBAR_L(cpu), phys_entry >> 0);
	iowrite32(cpucfg_base + CPUCFG_RVBAR_H(cpu), phys_entry >> 32);

	// 1) set the mode to aarch64
	ioset32(cpucfg_base + CPUCFG_CLX_CTRL0, CPUCFG_CLX_CTRL0_AARCH64(cpu));
}

static void a64_enable_cpu(uint cpu)
{
	void __iomem *prcm_base = ioremap(PRCM_BASE, PRCM_SIZE);
	void __iomem *cpucfg_base = ioremap(CPUCFG_BASE, CPUCFG_SIZE);
	void __iomem *r_cpucfg_base = ioremap(R_CPUCFG_BASE, R_CPUCFG_SIZE);

	// 0) set the entry point & mode
	smp_secondary_entry(cpucfg_base, cpu);

	// 1) assert CPU reset & clusetr power reset
	ioclr32(cpucfg_base + CPUCFG_CLX_RST_CTRL, 1 << cpu);
	udelay(10);
	ioclr32(r_cpucfg_base + R_CPUCFG_CLX_PWR_RST, 1 << cpu);
	udelay(10);

	// 2) release cpu power clamp
	power_switch_on(prcm_base, cpu);

	// 3) release power-off gating
	ioclr32(prcm_base + PRCM_CLX_PWROFF_GATING, 1 << cpu);
	udelay(20);

	// 4) deassert cluster power reset CPU reset
	ioset32(r_cpucfg_base + R_CPUCFG_CLX_PWR_RST, 1 << cpu);
	udelay(10);
	ioset32(cpucfg_base + CPUCFG_CLX_RST_CTRL, 1 << cpu);
	udelay(10);

	// 5) ...
	ioset32(cpucfg_base + CPUCFG_DBG0, 1 << cpu);
	udelay(10);
}

////////////////////////////////////////////////////////////////////////////////
static int a64_smp_init(void)
{
	return 0;
}

static int a64_smp_boot_cpu(struct thread *idle, uint cpu)
{

	// FIXME: gic_init_cpu(cpu);

	// FIXME: CCI-400

	a64_enable_cpu(cpu);

	return 0;
}

static int a64_smp_init_cpu(uint cpu)
{
	return 0;
}

struct smp_ops smp_ops = {
	.init	  = a64_smp_init,
	.boot_cpu = a64_smp_boot_cpu,
	.init_cpu = a64_smp_init_cpu,
};
