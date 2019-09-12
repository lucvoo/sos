#ifndef	_SOC_MODEL_H_
#define	_SOC_MODEL_H_

#include <soc/clock-config.h>


#ifndef __ASSEMBLY__
#include <io.h>

static inline void stm32_boot_flash(void __iomem *flash_base)
{
	unsigned int val;

	val = FLASH_ACR_LATENCY(WS);
	val |= FLASH_ACR_PRFTEN;
	val |= FLASH_ACR_ICEN;
	val |= FLASH_ACR_DCEN;
	iowrite32(flash_base + FLASH_ACR, val);
}

static inline void stm32_boot_config_clock(void __iomem *rcc_base)
{
	u32 cfgr, pll;

	// Configure
	cfgr = ioread(rcc_base + RCC_CFGR);
	cfgr |=  HPRE;
	cfgr |=  PPRE1;
	cfgr |=  PPRE2;
	iowrite32(rcc_base + RCC_CFGR, cfgr);

	// PLL
	pll = ioread32(rcc_base + RCC_PLLCFGR);
	pll |= RCC_PLLCFGR_PLLSRC;
	pll |= RCC_PLLCFGR_PLLQ(PLLQ);
	pll |= RCC_PLLCFGR_PLLP(PLLP);
	pll |= RCC_PLLCFGR_PLLN(PLLN);
	pll |= RCC_PLLCFGR_PLLM(PLLM);
	iowrite32(rcc_base + RCC_PLLCFGR, pll);
}
#endif

#endif
