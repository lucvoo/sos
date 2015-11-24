#include "clk-divider.h"
#include "clk-fixed.h"
#include "clk-ratio.h"
#include <init.h>
#include <io.h>


static struct clk_fixed		ckil;
static struct clk_fixed		osc;

static void imx6sx_clock_setup(void)
{
	// root clocks
	ckil.rate.rate = 1 << 15;
	clk_fixed_register(&ckil, "ckil");

	osc.rate.rate = 24000000;
	clk_fixed_register(&osc, "osc");
}
board_setup_initcall(imx6sx_clock_setup);
