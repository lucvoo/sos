#ifndef	_CLK_RATE_FIXED_OPS_H_
#define	_CLK_RATE_FIXED_OPS_H_

#include "clk-rate-fixed.h"


static inline unsigned long _clk_rate_fixed_get(struct clk_rate_fixed *c)
{
	return c->rate;
}


#define	DECLARE_CLKOPS_RATE(TYPE)				\
static unsigned long TYPE ## _get_rate(struct clk *clk)		\
{								\
	struct TYPE *c = container_of(clk, struct TYPE, clk);	\
								\
	return _clk_rate_fixed_get(&c->rate);			\
}

#endif
