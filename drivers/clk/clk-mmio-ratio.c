#include "clk-mmio-ratio.h"
#include "clk-gate-mmio-ops.h"
#include "clk-rate-ratio-ops.h"


DECLARE_CLKOPS_GATE(clk_mmio_ratio)
DECLARE_CLKOPS_RATE(clk_mmio_ratio)


static const struct clk_ops clk_mmio_ratio_ops = {
	.enable = clk_mmio_ratio_enable,
	.disable = clk_mmio_ratio_disable,
	.get_rate = clk_mmio_ratio_get_rate,
};


int clk_mmio_ratio_register(struct clk_mmio_ratio *c)
{
	return clk_register(&c->clk, &clk_mmio_ratio_ops);
}
