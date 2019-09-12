#include <soc/boot.h>
#include <soc/baseaddr.h>
#include <soc/rcc.h>
#include <soc/flash.h>
#include <soc/model.h>
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


void __init __stm32_boot_setup(void)
{
	void __iomem *flash_base = (void __iomem*) FLASH_BASE;
	void __iomem *rcc_base = (void __iomem*) RCC_BASE;
	unsigned int cr, cfgr;

	// Enable HSE
	ioset32(rcc_base + RCC_CR, RCC_CR_HSEON);
	do {
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_HSERDY) == 0);

	// FLASH
	stm32_boot_flash(flash_base);

	// Configure clocks
	stm32_boot_config_clock(rcc_base);

	// enable PLL
	ioset32(rcc_base + RCC_CR, RCC_CR_PLLON);
	do {
		cr = ioread32(rcc_base + RCC_CR);
	} while ((cr & RCC_CR_PLLRDY) == 0);

	// Finaly, set the system clock
	ioset32(rcc_base + RCC_CFGR, RCC_CFGR_SW_PLL);
	do {
		cfgr = ioread32(rcc_base + RCC_CFGR);
		cfgr &= RCC_CFGR_SWS_MSK;
	} while (cfgr != RCC_CFGR_SWS_PLL);
}
