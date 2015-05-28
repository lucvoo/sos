#ifndef	_SOC_CLOCK_H_
#define	_SOC_CLOCK_H_


#if	defined(CONFIG_STM32_CLK_12_168)
#define HSE	12
#define HCLK	168
#define HPRE	RCC_CFGR_HPRE_1
#define PPRE1	RCC_CFGR_PPRE1_4
#define PPRE2	RCC_CFGR_PPRE2_2
#define PLLM	6
#define PLLN	336
#define PLLP	4
#define PLLQ	14
#define WS	5
#elif	defined(CONFIG_STM32_CLK_8_180)
#define HSE	8
#define HCLK	180
#define HPRE	RCC_CFGR_HPRE_1
#define PPRE1	RCC_CFGR_PPRE1_4
#define PPRE2	RCC_CFGR_PPRE2_2
#define PLLM	4
#define PLLN	360
#define PLLP	4
#define PLLQ	15
#define WS	5
#endif

#endif
