#include "clk-fixed.h"
#include "clk-rate-fixed-ops.h"

/*
 * Define clocks with fixed rate and no gating.
 */

DECLARE_CLKOPS_RATE(clk_fixed)


static const struct clk_ops clk_fixed_ops = {
	.get_rate = clk_fixed_get_rate,
};


int clk_fixed_register(struct clk_fixed *c)
{
	return clk_register(&c->clk, &clk_fixed_ops);
}
