#include <soc/baseaddr.h>
#include <gpio.h>
#include <soc/fmc.h>
#include <soc/rcc.h>
#include <soc/clock-config.h>
#include <io.h>
#include <barrier.h>
#include <init.h>
#include <delay.h>
#include <memtester.h>

// Are needed:
//	FMC_A 0 - 12
//	FMC_D 0 - 7/15/31
//	FMC_A 14 - 15
//	FMC_NBL 0 - 0/1/3
//	SDCLK
//	SDCKE0/1
//	SDNE0/1
//	SDNRAS, SDNCAS
//	SDNWE

#ifdef CONFIG_STM32F429_DISCO
#define REFRESH_PERIOD	64000	// us
#define	BIT_ROW		12
#define	BIT_COL		8
#define	NBR_BANK	4
#define MBUS_WIDTH	16
#define	CAS_LAT		2
#define	RPIPE		FMC_SDCR_RPIPE(2)
#define	SDCLK		FMC_SDCR_SDCLK(2)
#define	NRFS		FMC_SDCMR_NRFS(4)
#define	MODE		((1 << 9) | (CAS_LAT << 4))
#define	SDRAM_CLK	(SYSCLK / 2)

static struct gpio_sdram {
	u8	port;
	u8	pin:4;
	u8	nbr:4;
} gpio_sdram[] = {
	{ 'B',  5, 2 },	// SDCKE1
	{ 'C',  0, 1 },	// SDNE1
	{ 'D',  0, 2 },	// D2,D3
	{ 'D',  8, 3 },	// D13-15
	{ 'D', 14, 2 },	// D0,D1
	{ 'E',  0, 2 },	// NBL0,1
	{ 'E',  7, 9 },	// D4-D12
	{ 'F',  0, 6 },	// A0-A5
	{ 'F', 11, 1 },	// SDNRAS
	{ 'F', 12, 4 },	// A6-A9
	{ 'G',  0, 2 },	// A10-A11
	{ 'G',  4, 2 },	// BA0,BA1
	{ 'G',  8, 1 },	// SDCLK
	{ 'G', 15, 1 },	// SDNCAS
};

#if 0
#endif
#endif

#define	CAS	FMC_SDCR_CAS(CAS_LAT)
#define	NB	FMC_SDCR_NB(NBR_BANK)
#define	NR	FMC_SDCR_NR(BIT_ROW)
#define	NC	FMC_SDCR_NC(BIT_COL)
#define	MWID	FMC_SDCR_MWID(MBUS_WIDTH)
#define	RCNT	FMC_SDRTR_COUNT(((SDRAM_CLK * REFRESH_PERIOD) / (1 << BIT_ROW)) - 20)

#define SDRAM_START	0xD0000000
#define SDRAM_SIZE	((NBR_BANK * MBUS_WIDTH/8) << (BIT_ROW + BIT_COL))

static void stm32_sdram_test(void)
{
	memtester((void*) SDRAM_START, SDRAM_SIZE, 1, 0);
}

static void __init stm32_sdram_busy_wait(void __iomem *fmcbase)
{
	while (ioread32(fmcbase + FMC_SDSR) & FMC_SDSR_BUSY)
		;
}

static void __init sdram_cmd(void __iomem *fmcbase, int bank, int nrfs, int cmd, int mode)
{
	u32 val = 0;

	if (bank & 1)
		val |= FMC_SDCMR_CTB1;
	if (bank & 2)
		val |= FMC_SDCMR_CTB2;

	val |= cmd;
	val |= FMC_SDCMR_NRFS(nrfs);
	val |= FMC_SDCMR_MRD(mode);

	stm32_sdram_busy_wait(fmcbase);

	ioset32(fmcbase + FMC_SDCMR, val);
}

static void __init stm32_sdram_init(void)
{
	void * __iomem rccbase = ioremap(RCC_BASE, RCC_SIZE);
	void * __iomem fmcbase = ioremap(FMC_BASE, FMC_SIZE);

	for (int i = ARRAY_SIZE(gpio_sdram); i--; ) {
		struct gpio_sdram *p = &gpio_sdram[i];
		char name[3] = { 'P', p->port, '\0' };
		for (int n = p->nbr; n--; ) {
			gpio_alt_func(gpio_get(name, p->pin + n), 12);
		}
	}

	ioset32(rccbase + RCC_AHB3ENR, RCC_AHB3EN_FMSCEN);

#if 1
	// Step 1
	ioclrset32(fmcbase + FMC_SDCR1, 0x00007FFF, SDCLK|RPIPE|FMC_SDCR_RBURST, 0);
	ioclrset32(fmcbase + FMC_SDCR2, 0x00007FFF, CAS|NB|MWID|NR|NC, 0);

	// Step 2
	ioclrset32(fmcbase + FMC_SDTR1, 0x0FFFFFFF, FMC_SDTR_TRC(5)|FMC_SDTR_TRP(1), 0);
	ioclrset32(fmcbase + FMC_SDTR2, 0x0FFFFFFF, FMC_SDTR_TMRD(1)|FMC_SDTR_TXSR(5)|FMC_SDTR_TRAS(3)|FMC_SDTR_TWR(1)|FMC_SDTR_TRCD(0), 0);

	// Step 3
	sdram_cmd(fmcbase, 2, 0, FMC_SDCMR_MODE_CLOCK_CFG_EN, 0);
	udelay(100);

	// Step 5
	sdram_cmd(fmcbase, 2, 0, FMC_SDCMR_MODE_PALL, 0);

	// Step 6
	sdram_cmd(fmcbase, 2, 4, FMC_SDCMR_MODE_AUTOREFRESH, 0);

	// Step 7
	sdram_cmd(fmcbase, 2, 0, FMC_SDCMR_MODE_LOAD_MODE, MODE);

	// Step 8
	stm32_sdram_busy_wait(fmcbase);
	ioset32(fmcbase + FMC_SDRTR, FMC_SDRTR_COUNT(RCNT));

	stm32_sdram_busy_wait(fmcbase);
#else
	iowrite32(fmcbase + FMC_SDCR1, 0x00001800);
	iowrite32(fmcbase + FMC_SDCR2, 0x000019D4);
	iowrite32(fmcbase + FMC_SDTR1, 0x00106000);
	iowrite32(fmcbase + FMC_SDTR2, 0x00010361);
	stm32_sdram_busy_wait(fmcbase);
	iowrite32(fmcbase + FMC_SDCMR, 0x00000009);
	for (int i = 0; i < 50000000; i++)
		asm volatile("nop");
	stm32_sdram_busy_wait(fmcbase);
	iowrite32(fmcbase + FMC_SDCMR, 0x0000000A);
	stm32_sdram_busy_wait(fmcbase);
	iowrite32(fmcbase + FMC_SDCMR, 0x000000EB);
	stm32_sdram_busy_wait(fmcbase);
	iowrite32(fmcbase + FMC_SDCMR, 0x0004600C);
	iowrite32(fmcbase + FMC_SDRTR, 1386 << 1);
	stm32_sdram_busy_wait(fmcbase);
#endif

	stm32_sdram_test();
}
board_initcall(stm32_sdram_init);
