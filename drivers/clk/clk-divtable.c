#include "clk-divtable.h"
#include <round.h>
#include <errno.h>
#include <bits.h>
#include <io.h>


static unsigned long clk_divtable_get_rate(struct clk *clk)
{
	struct clk_divtable *c = container_of(clk, struct clk_divtable, clk);
	unsigned long base = clk_get_rate(clk->parent);

	return base / c->div;
}

static int clk_divtable_set_rate(struct clk *clk, ulong target)
{
	struct clk_divtable *c = container_of(clk, struct clk_divtable, clk);
	ulong base = clk_get_rate(clk->parent);
	const struct divtable *tbl;
	uint div;

	if (target == 0)
		return -EINVAL;
	div = DIV_ROUND_UP(base, target);
	if (div == 0)
		return -EINVAL;

	for (tbl = &c->tbl[0]; ; tbl++) {
		if (tbl->div == 0)
			return -EINVAL;

		if (tbl->div == div)
			break;
	}

	c->set_val(c, tbl->val);
	c->div = div;
	return 0;
}


static const struct clk_ops clk_divtable_ops = {
	.get_rate = clk_divtable_get_rate,
	.set_rate = clk_divtable_set_rate,
};

int clk_divtable_register(struct clk_divtable *c)
{
	const struct divtable *tbl;
	uint val;
	int rc;

	if (!c->set_val)
		c->set_val = clk_divtable_set_val;
	if (!c->get_val)
		c->get_val = clk_divtable_get_val;

	val = c->get_val(c);
	for (tbl = &c->tbl[0]; ; tbl++) {
		if (tbl->div == 0)
			return -EINVAL;

		if (tbl->val == val)
			break;
	}
	c->div = tbl->div;

	rc = clk_register(&c->clk, &clk_divtable_ops);
	return rc;
}


// generic/default set/get divider's value
void clk_divtable_set_val(struct clk_divtable *c, uint val)
{
	u32 reg;
	reg = ioread32(c->iobase) & ~MASK(c->shift, c->width);
	reg |= val << c->shift;
	iowrite32(c->iobase, reg);
}

uint clk_divtable_get_val(struct clk_divtable *c)
{
	uint val;
	u32 reg;

	reg = ioread32(c->iobase);
	val = (reg >> c->shift) & MSK(c->width);
	return val;
}
