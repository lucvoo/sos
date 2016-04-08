#include <soc/ar933x.h>
#include <soc/reset.h>
#include <soc/ar71xx_regs.h>
#include <arch/iomap.h>
#include <init.h>
#include <io.h>


static uint ar933x_ref_freq(void)
{
	void __iomem *ioaddr = __ioremap(AR71XX_RESET_BASE);
	u32 status;

	status = ioread32(ioaddr + AR933X_RESET_REG_BOOTSTRAP);
	if (status & AR933X_BOOTSTRAP_REF_CLK_40)
		return 40 * 1000000;

	return 25 * 1000000;
}

void ar933x_get_freq(uint *cpu, uint *ddr, uint *ahb)
{
	void __iomem *pll_addr = __ioremap(AR71XX_PLL_BASE);
	uint ref = ar933x_ref_freq();
	uint ctrl = ioread32(pll_addr + AR933X_PLL_CLOCK_CTRL_REG);
	uint config = ioread32(pll_addr + AR933X_PLL_CPU_CONFIG_REG);
	uint pll;

	// is PLL bypassed?
	if (ctrl & AR933X_PLL_CLOCK_CTRL_BYPASS) {
		*cpu = ref;
		*ddr = ref;
		*ahb = ref;
		return;
	}

	pll = ref;
	pll /= AR933X_PLL_CPU_CONFIG_REFDIV(config);
	pll *= AR933X_PLL_CPU_CONFIG_DIVINT(config);
	pll >>= AR933X_PLL_CPU_CONFIG_OUTDIV(config);

	if (cpu)
		*cpu = pll / AR933X_PLL_CLOCK_CTRL_CPU_DIV(ctrl);
	if (ddr)
		*ddr = pll / AR933X_PLL_CLOCK_CTRL_DDR_DIV(ctrl);
	if (ahb)
		*ahb = pll / AR933X_PLL_CLOCK_CTRL_AHB_DIV(ctrl);
}
