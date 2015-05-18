#include <mach/boot.h>
#include <mach/baseaddr.h>
#include <mach/rcc.h>
#include <mach/flash.h>
#include <errno.h>
#include <init.h>
#include <io.h>


#define	CLOCK_WARMING_TIMEOUT	0x8000		// FIXME

#if 1	// FIXME: this is part of the board config
	// To be used with a 12MHz ext. clock
	// and give us a nice 168MHz for the system clock
#define	HPRE	RCC_CFGR_HPRE_1
#define PPRE1	RCC_CFGR_PPRE1_4
#define	PPRE2	RCC_CFGR_PPRE2_2
#define	PLLM	6
#define	PLLN	336
#define	PLLP	4
#define	PLLQ	14
#define	WS	5
#endif

static void iosl(void __iomem *addr, unsigned int val)
{
	iowrite32(addr, ioread32(addr) | val);
}

static void stm32_boot_flash(void)
{
	void __iomem *flash_base = (void __iomem*) FLASH_BASE;
	unsigned int val;

	val = FLASH_ACR_LATENCY(WS);
	val |= FLASH_ACR_PRFTEN;
	val |= FLASH_ACR_ICEN;
	val |= FLASH_ACR_DCEN;
	iowrite32(flash_base + FLASH_ACR, val);
}

int __init __stm32_boot_setup(void)
{
	void __iomem *rcc_base = (void __iomem*) RCC_BASE;
	unsigned int cr, cfgr, pll;
	unsigned int val;
	int n;

	// Enable HSE
	iosl(rcc_base + RCC_CR, RCC_CR_HSEON);
	n = CLOCK_WARMING_TIMEOUT;
	do {
		if (--n <= 0)
			return -EIO;
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_HSERDY) == 0);

	// Configure
	cfgr = 0;
	cfgr |=  HPRE;
	cfgr |=  PPRE1;
	cfgr |=  PPRE2;
	iowrite32(rcc_base + RCC_CFGR, cfgr);

	// PLL
	pll = RCC_PLLCFGR_PLLSRC;
	pll |= RCC_PLLCFGR_PLLQ(PLLQ);
	pll |= RCC_PLLCFGR_PLLP(PLLP);
	pll |= RCC_PLLCFGR_PLLN(PLLN);
	pll |= RCC_PLLCFGR_PLLM(PLLM);
	iowrite32(rcc_base + RCC_PLLCFGR, pll);

	// enable PLL
	iosl(rcc_base + RCC_CR, RCC_CR_PLLON);
	do {
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_PLLRDY) == 0);

	// FLASH
	stm32_boot_flash();

	// Finaly, set the system clock
	iosl(rcc_base + RCC_CFGR, RCC_CFGR_SW_PLL);
	do {
		cfgr = ioread32(rcc_base + RCC_CFGR);
		val = cfgr & RCC_CFGR_SWS_MSK;
	} while (val != RCC_CFGR_SWS(RCC_CFGR_SW_PLL));

	return 0;
}
