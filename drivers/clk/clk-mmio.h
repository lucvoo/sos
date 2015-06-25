#ifndef	_CLK_MMIO_H_
#define	_CLK_MMIO_H_

#include "clk-gate-mmio.h"

struct clk_mmio {
	struct clk		clk;
	struct clk_gate_mmio	gate;
};

int clk_mmio_register(struct clk_mmio *c);

#endif
