#ifndef	_CLK_FIXED_H_
#define	_CLK_FIXED_H_

#include "clk-rate-fixed.h"

struct clk_fixed {
	struct clk		clk;
	struct clk_rate_fixed	rate;
};

int clk_fixed_register(struct clk_fixed *c);

static int clk_fixed_init(struct clk_fixed *c, const char *name, ulong rate)
{
	c->rate.rate = rate;
	c->clk.name = name;
	return clk_fixed_register(c);
}

#endif
