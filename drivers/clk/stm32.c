#include <soc/clock-config.h>
#include <soc/rcc.h>
#include "clk-fixed.h"
#include "clk-ratio.h"
#include "clk-mmio.h"
#include <init.h>
#include <io.h>


static struct clk_fixed sysclk;
static struct clk_ratio hclk;
static struct clk_ratio systickclk;
static struct clk_ratio apb1clk;
static struct clk_ratio tim1clk;
static struct clk_mmio timer2clk;


#define	FREQ_SYSCLK	(SYSCLK * 1000000)

static void stm32_clock_setup(void)
{
	void __iomem *rccbase;

	rccbase = ioremap(RCC_BASE, RCC_SIZE);

	sysclk.rate.rate = FREQ_SYSCLK;
	clk_fixed_register(&sysclk, "sysclk");

	hclk.rate.mult = 1;
	hclk.rate.div = AHBDIV;
	hclk.clk.parent = &sysclk.clk;
	hclk.clk.name = "ahb";
	clk_ratio_register(&hclk);


	systickclk.rate.mult = 1;
	systickclk.rate.div = 8;
	systickclk.clk.parent = &hclk.clk;
	systickclk.clk.name = "systickclk";
	clk_ratio_register(&systickclk);


	apb1clk.rate.mult = 1;
	apb1clk.rate.div = APB1DIV;
	apb1clk.clk.parent = &hclk.clk;
	apb1clk.clk.name = "apb1clk";
	clk_ratio_register(&apb1clk);

	tim1clk.rate.mult = 2;
	tim1clk.rate.div = 1;
	tim1clk.clk.parent = &apb1clk.clk;
	tim1clk.clk.name = "tim1clk";
	clk_ratio_register(&tim1clk);

	timer2clk.gate.reg = rccbase + RCC_APB1ENR;
	timer2clk.gate.bit = RCC_APB1EN_TIMER2_BIT;
	timer2clk.clk.parent = &tim1clk.clk;
	timer2clk.clk.name = "timer2clk";
	clk_mmio_register(&timer2clk);


	//apb2clk.rate.mult = 1;
	//apb2clk.rate.div = APB2DIV;
	//apb2clk.clk.parent = &hclk;
	//apb2clk.clk.name = "apb2clk";
	//clk_ratio_register(&apb2clk);

	//tim2clk.rate.mult = 2;
	//tim2clk.rate.div = 1;
	//tim2clk.clk.parent = &apb2clk.clk;
	//tim2clk.clk.name = "tim2clk";
	//clk_ratio_register(&tim2clk);
}
board_setup_initcall(stm32_clock_setup);
