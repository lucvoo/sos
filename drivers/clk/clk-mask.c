#include "clk-mask.h"
#include "clk-gate-mask-ops.h"


DECLARE_CLKOPS_GATE(clk_mask)


static const struct clk_ops clk_mask_ops = {
	.enable = clk_mask_enable,
	.disable = clk_mask_disable,
};


int clk_mask_register(struct clk_mask *c)
{
	return clk_register(&c->clk, &clk_mask_ops);
}
