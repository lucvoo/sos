#ifndef	_CLK_FIXED_H_
#define	_CLK_FIXED_H_

#include "clk-rate-fixed.h"

struct clk_fixed {
	struct clk		clk;
	struct clk_rate_fixed	rate;
};

int clk_fixed_register(struct clk_fixed *c);

#endif
