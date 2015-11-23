#ifndef	_CLK_MASK_H_
#define	_CLK_MASK_H_

#include "clk-gate-mask.h"

struct clk_mask {
	struct clk		clk;
	struct clk_gate_mask	gate;
};

int clk_mask_register(struct clk_mask *c);

#endif
