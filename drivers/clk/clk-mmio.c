#include "clk-mmio.h"
#include "clk-gate-mmio-ops.h"


DECLARE_CLKOPS_GATE(clk_mmio)


static const struct clk_ops clk_mmio_ops = {
	.enable = clk_mmio_enable,
	.disable = clk_mmio_disable,
};


int clk_mmio_register(struct clk_mmio *c)
{
	return clk_register(&c->clk, &clk_mmio_ops);
}
