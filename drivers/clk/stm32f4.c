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
static struct clk_ratio apb1timclk;
static struct clk_mmio timer2clk;


#define	FREQ_SYSCLK	(SYSCLK * 1000000)

static void stm32_clock_setup(void)
{
	void __iomem *rccbase;

	rccbase = ioremap(RCC_BASE, RCC_SIZE);

	clk_fixed_init(&sysclk, "sysclk", FREQ_SYSCLK);

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

	apb1timclk.rate.mult = APB1TIM_MUL;
	apb1timclk.rate.div = 1;
	apb1timclk.clk.parent = &apb1clk.clk;
	apb1timclk.clk.name = "apb1timclk";
	clk_ratio_register(&apb1timclk);

	timer2clk.gate.reg = rccbase + RCC_APB1ENR;
	timer2clk.gate.bit = RCC_APB1EN_TIMER2_BIT;
	timer2clk.clk.parent = &apb1timclk.clk;
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
