#include "clk-mux-mmio.h"
#include <errno.h>
#include <io.h>


static inline void clk_mux_mmio_init_parent(struct clk_mux_mmio *c)
{
	uint nbr = 1 << c->width;
	uint mask = ((1 << nbr) - 1);
	uint idx;
	u32 val;

	val = ioread32(c->reg);
	idx = (val >> c->shift) & mask;
	c->clk.parent =  c->parents[idx];
}

static inline int clk_mux_mmio_set_parent(struct clk *clk, struct clk *parent)
{
	struct clk_mux_mmio *c = container_of(clk, struct clk_mux_mmio, clk);
	uint nbr = 1 << c->width;
	uint mask = ((1 << nbr) - 1) << c->shift;
	uint idx;
	u32 val;

	for (idx = 0; idx < nbr; idx++) {
		if (parent == c->parents[idx])
			goto found;
	}
	return -EINVAL;

found:
	val = ioread32(c->reg);
	val = (val & ~mask) | (idx << c->shift);
	iowrite32(c->reg, val);
	c->clk.parent = parent;
	return 0;
}


static const struct clk_ops clk_mux_mmio_ops = {
};


int clk_mux_mmio_register(struct clk_mux_mmio *c, struct clk *parent)
{
	int rc = 0;

	if (parent)
		rc = clk_mux_mmio_set_parent(&c->clk, parent);
	else
		clk_mux_mmio_init_parent(c);
	if (!rc)
		rc = clk_register(&c->clk, &clk_mux_mmio_ops);

	return rc;
}
