#ifndef	_MACH_RCC_H_
#define	_MACH_RCC_H_

#if   defined(CONFIG_STM32F10X)

#define RCC_CR		0x00
#define   RCC_CR_PLLRDY		(1 << 25)
#define   RCC_CR_PLLON		(1 << 24)
#define   RCC_CR_CSSON		(1 << 19)
#define   RCC_CR_HSEBPY		(1 << 18)
#define   RCC_CR_HSERDY		(1 << 17)
#define   RCC_CR_HSEON		(1 << 16)
#define   RCC_CR_HSIRDY		(1 << 1)
#define   RCC_CR_HSION		(1 << 0)

#define RCC_CFGR	0x04
#define   RCC_CFGR_USBPRE	(1 << 22)
#define   RCC_CFGR_PLLMUL(X)	(((X)-2) << 18)
#define   RCC_CFGR_PLLXTPRE	(1 << 17)
#define   RCC_CFGR_PLLSRC_HSE	(1 << 16)
#define   RCC_CFGR_PPRE2(X)	((X) << 13)
#define   RCC_CFGR_PPRE1(X)	((X) << 10)
#define   RCC_CFGR_PPRE_MSK	0x7
#define   RCC_CFGR_HPRE(X)	((X) << 4)
#define   RCC_CFGR_HPRE_MSK	RCC_CFGR_HPRE(0xF)
#define   RCC_CFGR_SWS(X)	((X) << 2)
#define   RCC_CFGR_SW(X)	((X) << 0)
#define   RCC_CFGR_SW_HSI	0x0
#define   RCC_CFGR_SW_HSE	0x1
#define   RCC_CFGR_SW_PLL	0x2
#define   RCC_CFGR_SW_MSK	0x3
#define   RCC_CFGR_SWS_MSK	RCC_CFGR_SWS(RCC_CFGR_SW_MSK)
#define   RCC_CFGR_SWS_PLL	RCC_CFGR_SWS(RCC_CFGR_SW_PLL)
#define   RCC_CFGR_RESERVED	0xf8800000

#define RCC_CIR		0x08

#define RCC_APB2ENR	0x18
#define   RCC_APB2EN_USART1EN	(1 << 14)
#define   RCC_APB2EN_IOPGEN	(1 <<  8)
#define   RCC_APB2EN_IOPFEN	(1 <<  7)
#define   RCC_APB2EN_IOPEEN	(1 <<  6)
#define   RCC_APB2EN_IOPDEN	(1 <<  5)
#define   RCC_APB2EN_IOPCEN	(1 <<  4)
#define   RCC_APB2EN_IOPBEN	(1 <<  3)
#define   RCC_APB2EN_IOPAEN	(1 <<  2)
#define   RCC_APB2EN_AFIOEN	(1 <<  0)
#define   RCC_APB2EN_GPIOEN(X)	(1 << ((X)+2))

#define RCC_GPIOENR	RCC_APB2ENR
#define   RCC_GPIOEN(X)		RCC_APB2EN_GPIOEN(X)

#elif defined(CONFIG_STM32F4XX)

/*
 * Frequencies:
 *	PLLvco: PLLin * (N/M)
 *	PLLCLK: PLLvco / P
 *	PLL48:  PLLvco / Q	(USB OTG & RNG)
 * Constraints:
 *	192 <= N <= 432
 *	  2 <= Q <=  15
 *	  2 <= M <=  63
 *	       P = 2, 4, 6 or 8
 *  PLLin/2 <= M <= PLLin	(ideally M == PLLin/2 (reduced PLL jitter))
 *  PLL48 = 48 MHz
 *
 * For exemple:
 *	1) PLLin = 12MHz, Q = 14, P = 4, M = 6, N = 336
 *	   => PLLCLK = 168MHz, PLL48 = 48MHz, PLLvco = 672MHz
 *	2) PLLin = 12MHZ, Q = 5, P = 2, M = 12, N = 240
 *	   => PLLCLK = 120MHz
 */

#define	RCC_CR		0x00
#define	RCC_CR_PLLI2SRDY	(1 << 27)
#define	RCC_CR_PLLI2SON		(1 << 26)
#define	RCC_CR_PLLRDY		(1 << 25)
#define	RCC_CR_PLLON		(1 << 24)
#define	RCC_CR_CSSON		(1 << 19)
#define	RCC_CR_HSEBPY		(1 << 18)
#define	RCC_CR_HSERDY		(1 << 17)
#define	RCC_CR_HSEON		(1 << 16)
#define	RCC_CR_HSIRDY		(1 << 1)
#define	RCC_CR_HSION		(1 << 0)

#define	RCC_PLLCFGR	0x04
#define	RCC_PLLCFGR_PLLM(X)	((X) << 0)
#define	RCC_PLLCFGR_PLLN(X)	((X) << 6)
#define	RCC_PLLCFGR_PLLP(X)	(((X)/2 - 1) << 16)
#define	RCC_PLLCFGR_PLLSRC	(1 << 22)
#define	RCC_PLLCFGR_PLLQ(X)	((X) << 24)

#define	RCC_CFGR	0x08
#define	RCC_CFGR_SW_HSI		0x0
#define	RCC_CFGR_SW_HSE		0x1
#define	RCC_CFGR_SW_PLL		0x2
#define	RCC_CFGR_SW_MSK		0x3
#define	RCC_CFGR_SW(X)		((X) << 0)
#define	RCC_CFGR_SWS(X)		((X) << 2)
#define	RCC_CFGR_SWS_MSK	RCC_CFGR_SWS(RCC_CFGR_SW_MSK)
#define	RCC_CFGR_SWS_PLL	RCC_CFGR_SWS(RCC_CFGR_SW_PLL)
#define	RCC_CFGR_HPRE(X)	((X) << 4)
#define	RCC_CFGR_HPRE_MSK	RCC_CFGR_HPRE(0XF)
#define	RCC_CFGR_HPRE_1		RCC_CFGR_HPRE(0)
#define	RCC_CFGR_PPRE1(X)	((X) << 10)
#define	RCC_CFGR_PPRE1_MSK	RCC_CFGR_PPRE1(0x7)
#define	RCC_CFGR_PPRE1_4	RCC_CFGR_PPRE1(0x5)
#define	RCC_CFGR_PPRE2(X)	((X) << 13)
#define	RCC_CFGR_PPRE2_MSK	RCC_CFGR_PPRE2(0x7)
#define	RCC_CFGR_PPRE2_2	RCC_CFGR_PPRE2(0x4)
#define	RCC_CFGR_RTCPRE(X)	((X) << 16)
#define	RCC_CFGR_RTCPRE_MSK	RCC_CFGR_RTCPRE(0x1F)

#define	RCC_CIR		0x0C

#define	RCC_AHB1ENR	0x30
#define	RCC_AHB1EN_GPIOEN(PORT)	(1 << (PORT))

#define	RCC_GPIOENR	RCC_AHB1ENR
#define	  RCC_GPIOEN(X)		RCC_AHB1EN_GPIOEN(X)

#define	RCC_APB1ENR	0x40
#define	RCC_APB1EN_TIMER2_BIT	0
#define	RCC_APB1EN_TIMER2	(1 <<  RCC_APB1EN_TIMER2_BIT)
#define	RCC_APB1EN_USART3EN	(1 << 18)

#define	RCC_APB2ENR	0x44
#define	RCC_APB2EN_USART1EN	(1 << 4)
#endif

#endif
