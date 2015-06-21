#ifndef	_CLK_RATIO_H_
#define	_CLK_RATIO_H_

#include "clk-rate-ratio.h"

struct clk_ratio {
	struct clk		clk;
	struct clk_rate_ratio	rate;
};

int clk_ratio_register(struct clk_ratio *c);

#endif
