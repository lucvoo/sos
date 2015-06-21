#ifndef	_CLK_MMIO_RATIO_H_
#define	_CLK_MMIO_RATIO_H_

#include "clk-gate-mmio.h"
#include "clk-rate-ratio.h"

struct clk_mmio_ratio {
	struct clk		clk;
	struct clk_gate_mmio	gate;
	struct clk_rate_ratio	rate;
};

int clk_mmio_ratio_register(struct clk_mmio_ratio *c);

#endif
