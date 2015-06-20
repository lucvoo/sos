#ifndef	_CLK_GATE_MMIO_OPS_H_
#define	_CLK_GATE_MMIO_OPS_H_

#include "clk-gate-mmio.h"
#include <io.h>

static inline void _clk_gate_mmio_xable(struct clk_gate_mmio *c, int enable)
{
	unsigned int val;
	unsigned int msk;


	msk = (1 << c->bit);
	val = ioread32(c->reg);

	if (enable ^ c->neg_logic)
		val |=  msk;
	else
		val &= ~msk;

	iowrite32(c->reg, val);
}


#define	DECLARE_CLKOPS_GATE(TYPE)				\
static int TYPE ## _enable(struct clk *clk)			\
{								\
	struct TYPE *c = container_of(clk, struct TYPE, clk);	\
								\
	_clk_gate_mmio_xable(&c->gate, 1);			\
	return 0;						\
}								\
								\
static void TYPE ## _disable(struct clk *clk)			\
{								\
	struct TYPE *c = container_of(clk, struct TYPE, clk);	\
								\
	_clk_gate_mmio_xable(&c->gate, 0);			\
}

#endif
