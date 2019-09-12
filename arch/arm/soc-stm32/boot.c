#include <soc/boot.h>
#include <soc/baseaddr.h>
#include <soc/rcc.h>
#include <soc/flash.h>
#include <soc/clock-config.h>
#include <errno.h>
#include <init.h>
#include <io.h>


/*
 * To reflash, on openocd:
 *	reset halt
 *	flash write_image erase <filename.elf>
 *
 * Note: it may be needed to first unlock the board, for example:
 *	reset halt
 *	stm32f2x unlock 0
 *	mww 0x40023C08 0x08192A3B; mww 0x40023C08 0x4C5D6E7F; mww 0x40023C14 0x0fffaaed
 *	flash write_image erase <filename.elf>
 */


#define	CLOCK_WARMING_TIMEOUT	0x8000		// FIXME


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
	ioset32(rcc_base + RCC_CR, RCC_CR_HSEON);
	n = CLOCK_WARMING_TIMEOUT;
	do {
		if (--n <= 0)
			return -EIO;
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_HSERDY) == 0);

	// FLASH
	stm32_boot_flash();

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
	ioset32(rcc_base + RCC_CR, RCC_CR_PLLON);
	do {
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_PLLRDY) == 0);

	// Finaly, set the system clock
	ioset32(rcc_base + RCC_CFGR, RCC_CFGR_SW_PLL);
	do {
		cfgr = ioread32(rcc_base + RCC_CFGR);
		val = cfgr & RCC_CFGR_SWS_MSK;
	} while (val != RCC_CFGR_SWS(RCC_CFGR_SW_PLL));

	return 0;
}
