#ifndef	_SOC_MODEL_H_
#define	_SOC_MODEL_H_

#define	NVIC_NBR_IRQ	49
#define	WS		2	// wait state for flash access

#define	HSE_FREQ	 8	// MHz
#define	CLK_FREQ	72	// MHz
#define	PLLM		(CLK_FREQ / HSE_FREQ)
#define	PPRE1		0b100	// APB clk = PCLK1 (72MHz) / 2


#ifndef __ASSEMBLY__
#include <io.h>

static void stm32_boot_flash(void __iomem *flash_base)
{
	u32 val;

	val = FLASH_ACR_LATENCY(WS);
	val |= FLASH_ACR_PRFTBE;
	iowrite32(flash_base + FLASH_ACR, val);
}

static inline void stm32_boot_config_clock(void __iomem * rcc_base)
{
	u32 cfgr = ioread32(rcc_base + RCC_CFGR);
	cfgr |= RCC_CFGR_PLLSRC_HSE;
	cfgr |= RCC_CFGR_PLLMUL(PLLM);
	cfgr |= RCC_CFGR_PPRE1(PPRE1);
	iowrite32(rcc_base + RCC_CFGR, cfgr);
}
#endif

#endif
