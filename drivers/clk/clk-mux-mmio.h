#ifndef	_CLK_MUX_MMIO_H_
#define	_CLK_MUX_MMIO_H_

#include "clk-core.h"
#include <iomem.h>


struct clk_mux_mmio {
	struct clk	clk;
	void __iomem	*reg;
	struct clk * const *parents;
	u8		shift;
	u8		width;
};

int clk_mux_mmio_register(struct clk_mux_mmio *c, struct clk *parent);

#endif
