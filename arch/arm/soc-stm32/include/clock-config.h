#ifndef	_SOC_CLOCK_H_
#define	_SOC_CLOCK_H_

#include <soc/rcc.h>

/*
 * - f(VCOin)  = f(PLLin) / PLLM
 * - f(VCOout) = f(VCOin) × PLLN	(192 ≤ VCOout ≤ 432)
 * - f(PLLout) = f(VCOout) / PLLP	(< 168 or 180MHz)
 * - f(USB OTG FS, SDIO, RNG clock output) = f(VCOout) / PLLQ
 *
 * Normally, PLLin is HSE (High-Speed External) and
 * SYSCLK is PLLout
 */

#if 0
#elif CONFIG_STM32_PLL_MAX >= 168
#define HSE	CONFIG_STM32_HSE_FREQ
#define PLLM	(HSE / 2)
#define PLLN	168
#if CONFIG_STM32_CLK_MAX < 168
#define PLLP	3
#else
#define PLLP	2
#endif
#define PLLQ	7
#define	AHBDIV	1
#define HPRE	RCC_CFGR_HPRE_1
#define	APB1DIV	4
#define PPRE1	RCC_CFGR_PPRE1_4
#define	APB2DIV	2
#define PPRE2	RCC_CFGR_PPRE2_2
#define WS	5
#endif

#define	VCOIN	(HSE / PLLM)
#define	VCOOUT	(VCOIN * PLLN)
#define	PLLOUT	(VCOOUT / PLLP)
#define SYSCLK	(PLLOUT)

#if VCOOUT < 192 || VCOOUT > 432
#error "invalid VCOout"
#endif
#if PLLOUT > CONFIG_STM32_CLK_MAX
#error "PLLout is too high"
#endif

#define	HCLK		(SYSCLK/AHBDIV)
#define	APB1CLK		(HCLK/APB1DIV)
#define	APB2CLK		(HCLK/APB2DIV)
#define	SYSTICKCLK	(HCLK/8)

#define	APB1TIM_MUL	((APB1DIV == 1) ? 1 : 2)
#define	APB2TIM_MUL	((APB2DIV == 1) ? 1 : 2)

#endif
