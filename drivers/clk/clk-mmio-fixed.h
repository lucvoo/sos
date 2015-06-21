#ifndef	_CLK_MMIO_FIXED_H_
#define	_CLK_MMIO_FIXED_H_

#include "clk-gate-mmio.h"
#include "clk-rate-fixed.h"

struct clk_mmio_fixed {
	struct clk		clk;
	struct clk_gate_mmio	gate;
	struct clk_rate_fixed	rate;
};

int clk_mmio_fixed_register(struct clk_mmio_fixed *c);

#endif
