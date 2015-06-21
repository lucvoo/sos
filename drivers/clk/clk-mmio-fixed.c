#include "clk-mmio-fixed.h"
#include "clk-gate-mmio-ops.h"
#include "clk-rate-fixed-ops.h"


DECLARE_CLKOPS_GATE(clk_mmio_fixed)
DECLARE_CLKOPS_RATE(clk_mmio_fixed)


static const struct clk_ops clk_mmio_fixed_ops = {
	.enable = clk_mmio_fixed_enable,
	.disable = clk_mmio_fixed_disable,
	.get_rate = clk_mmio_fixed_get_rate,
};


int clk_mmio_fixed_register(struct clk_mmio_fixed *c)
{
	return clk_register(&c->clk, &clk_mmio_fixed_ops);
}
