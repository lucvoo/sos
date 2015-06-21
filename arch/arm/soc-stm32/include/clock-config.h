#ifndef	_SOC_CLOCK_H_
#define	_SOC_CLOCK_H_

#include <soc/rcc.h>

#if	defined(CONFIG_STM32_CLK_12_168)
#define HSE	12
#define SYSCLK	168
#define	AHBDIV	1
#define HPRE	RCC_CFGR_HPRE_1
#define	APB1DIV	4
#define PPRE1	RCC_CFGR_PPRE1_4
#define	APB2DIV	2
#define PPRE2	RCC_CFGR_PPRE2_2
#define PLLM	6
#define PLLN	336
#define PLLP	4
#define PLLQ	14
#define WS	5
#elif	defined(CONFIG_STM32_CLK_8_180)
#define HSE	8
#define SYSCLK	180
#define	AHBDIV	1
#define HPRE	RCC_CFGR_HPRE_1
#define	APB1DIV	4
#define PPRE1	RCC_CFGR_PPRE1_4
#define	APB2DIV	2
#define PPRE2	RCC_CFGR_PPRE2_2
#define PLLM	4
#define PLLN	360
#define PLLP	4
#define PLLQ	15
#define WS	5
#endif

#define	HCLK		(SYSCLK/AHBDIV)
#define	APB1CLK		(HCLK/APB1DIV)
#define	APB2CLK		(HCLK/APB2DIV)
#define	TIM1CLK		(APB1CLK*2)
#define	TIM2CLK		(APB2CLK*2)
#define	SYSTICKCLK	(HCLK/8)

#endif
