#ifndef	_CLK_DIVIDER_H_
#define	_CLK_DIVIDER_H_

#include "clk-core.h"
#include <iomem.h>


struct clk_divider {
	struct clk	clk;
	void __iomem	*iobase;
	uint		(*get_div)(struct clk_divider *clk);
	int		(*set_div)(struct clk_divider *clk, uint div);
	uint		div;
	u8		shift;
	u8		width;
	u8		offset;
	u8		scale;
};

uint clk_divider_get_div(struct clk_divider *clk);
int clk_divider_set_div(struct clk_divider *clk, uint div);

int clk_divider_register(struct clk_divider *c);

#endif
