#ifndef	_CLK_DIVTABLE_H_
#define	_CLK_DIVTABLE_H_

#include "clk-core.h"
#include <iomem.h>


struct divtable {
	uint	val;
	ulong	div;
};

struct clk_divtable {
	struct clk	clk;
	void __iomem	*iobase;
	uint		(*get_val)(struct clk_divtable *clk);
	void		(*set_val)(struct clk_divtable *clk, uint val);
	const struct divtable *tbl;
	uint		div;
	u8		shift;
	u8		width;
};


uint clk_divtable_get_val(struct clk_divtable *clk);
void clk_divtable_set_val(struct clk_divtable *clk, uint val);

int clk_divtable_register(struct clk_divtable *c);

#endif
