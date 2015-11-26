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
	clk_fixed_init(&ckil, "ckil", 1 << 15);
	clk_fixed_init(&osc,  "osc",   24000000);

}
board_setup_initcall(imx6sx_clock_setup);
