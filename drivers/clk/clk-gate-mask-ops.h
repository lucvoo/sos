#ifndef	_CLK_GATE_MASK_OPS_H_
#define	_CLK_GATE_MASK_OPS_H_

#include "clk-gate-mask.h"
#include <io.h>


static inline void _clk_gate_mask_xable(struct clk_gate_mask *c, int enable)
{
	unsigned int val = ioread32(c->reg);
	unsigned int msk = c->msk;

	if (enable)
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
	_clk_gate_mask_xable(&c->gate, 1);			\
	return 0;						\
}								\
								\
static void TYPE ## _disable(struct clk *clk)			\
{								\
	struct TYPE *c = container_of(clk, struct TYPE, clk);	\
								\
	_clk_gate_mask_xable(&c->gate, 0);			\
}

#endif
