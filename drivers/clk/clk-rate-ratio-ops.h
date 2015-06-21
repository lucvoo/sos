#ifndef	_CLK_RATE_RATIO_OPS_H_
#define	_CLK_RATE_RATIO_OPS_H_

#include "clk-rate-ratio.h"


static inline unsigned long _clk_rate_ratio_get(struct clk *parent, struct clk_rate_ratio *c)
{
	unsigned long rate = clk_get_rate(parent);

	return (rate * c->mult)/c->div;
}


#define	DECLARE_CLKOPS_RATE(TYPE)				\
static unsigned long TYPE ## _get_rate(struct clk *clk)		\
{								\
	struct TYPE *c = container_of(clk, struct TYPE, clk);	\
								\
	return _clk_rate_ratio_get(clk->parent, &c->rate);	\
}

#endif
