#include "clk-ratio.h"
#include "clk-rate-ratio-ops.h"

/*
 * Define clocks with ratio rate and no gating.
 */

DECLARE_CLKOPS_RATE(clk_ratio)


static const struct clk_ops clk_ratio_ops = {
	.get_rate = clk_ratio_get_rate,
};


int clk_ratio_register(struct clk_ratio *c)
{
	return clk_register(&c->clk, &clk_ratio_ops);
}
