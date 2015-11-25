#include "clk-fixed.h"
#include "clk-ratio.h"
#include "clk-divider.h"
#include <soc/cgu.h>
#include <errno.h>
#include <init.h>
#include <io.h>


static struct clk_fixed rtc;
static struct clk_fixed ext;

static struct clk_ratio sclk_m;		// FIXME: adjustable PLL

static struct clk_divider   msc0;


static void jz4780_clock_setup(void)
{
	void __iomem* cgubase = ioremap(CGU_BASE, 0x100);

	rtc.rate.rate = 1 << 15;
	clk_fixed_register(&rtc, "rtc");

	ext.rate.rate = 48000000;
	clk_fixed_register(&ext, "ext");

	sclk_m.rate.mult = 25;		// FIXME: adjustable PLL
	sclk_m.rate.div = 1;
	sclk_m.clk.parent = &ext.clk;
	clk_ratio_register(&sclk_m, "sclk_m");

	#define	msc_mux	sclk_m		// FIXME: MUX(sclk_m, sclk_a)

	msc0.iobase = cgubase + CGU_MSC0CDR;
	msc0.shift = 0;
	msc0.width = 8;
	msc0.offset = 1;
	msc0.scale = 2;
	msc0.clk.parent = &msc_mux.clk;
	clk_divider_register(&msc0, "msc0");
}
board_setup_initcall(jz4780_clock_setup);
