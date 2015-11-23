#include "clk-divider.h"
#include <round.h>
#include <errno.h>
#include <bits.h>
#include <io.h>


static unsigned long clk_divider_get_rate(struct clk *clk)
{
	struct clk_divider *c = container_of(clk, struct clk_divider, clk);
	unsigned long base = clk_get_rate(clk->parent);

	return base / c->div;
}

static int clk_divider_set_rate(struct clk *clk, ulong target)
{
	struct clk_divider *c = container_of(clk, struct clk_divider, clk);
	ulong base = clk_get_rate(clk->parent);
	uint div = DIV_ROUND_UP(base, target);
	int rc;

	if (target == 0)
		return -EINVAL;
	if (div == 0)
		return -EINVAL;

	rc = c->set_div(c, div);
	if (!rc)
		c->div = div;

	return rc;
}



static const struct clk_ops clk_divider_ops = {
	.get_rate = clk_divider_get_rate,
	.set_rate = clk_divider_set_rate,
};


int clk_divider_register(struct clk_divider *c)
{
	int rc;

	if (!c->set_div)
		c->set_div = clk_divider_set_div;
	if (!c->get_div)
		c->get_div = clk_divider_get_div;
	if (c->scale == 0)
		c->scale = 1;
	c->div = c->get_div(c);

	rc = clk_register(&c->clk, &clk_divider_ops);
	return rc;
}


// generic/default set/get divider
int clk_divider_set_div(struct clk_divider *c, uint div)
{
	u32 val;

	div = (div / c->scale) - c->offset;
	if (div >= (1U << c->shift))
		return -EINVAL;

	val = ioread32(c->iobase) & ~MASK(c->shift, c->width);
	val |= div << c->shift;
	iowrite32(c->iobase, val);
	return 0;
}

uint clk_divider_get_div(struct clk_divider *c)
{
	u32 val;
	uint div;

	val = (ioread32(c->iobase) >> c->shift) & MSK(c->width);
	div = (val + c->offset) * c->scale;
	return div;
}
