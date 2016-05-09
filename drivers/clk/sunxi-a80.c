#include "clk-fixed.h"
#include <errno.h>
#include <init.h>
#include <io.h>


static struct clk_fixed osc24M;
static struct clk_fixed osc32k;


static void sunxi_a80_clock_setup(void)
{
	clk_fixed_init(&osc32k, "osc32k", 1 << 15);
	clk_fixed_init(&osc24M, "osc24M", 24000000);
}
board_setup_initcall(sunxi_a80_clock_setup);
