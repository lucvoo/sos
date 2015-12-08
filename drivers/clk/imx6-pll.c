#include "imx6-pll.h"
#include <soc/ccm.h>
#include <round.h>
#include <errno.h>
#include <bits.h>
#include <io.h>


#define	REG_VAL		0x0
#define	REG_SET		0x4
#define	REG_CLR		0x8
#define	REG_XOR		0xC


static int clk_imx6pll_wait_locked(struct clk_imx6pll *c)
{
	// FIXME: need proper timeout handling
	// FIXME: need .prepare instead of .enable

	while (1) {
		if (ioread32(c->reg + REG_VAL) & PLL_LOCKED)
			return 0;
	}
}

static int clk_imx6pll_enable(struct clk *clk)
{
	struct clk_imx6pll *c = container_of(clk, struct clk_imx6pll, clk);
	int rc;

	iowrite32(c->reg + REG_CLR, PLL_POWERDOWN);

	rc = clk_imx6pll_wait_locked(c);
	return rc;
}

static void clk_imx6pll_disable(struct clk *clk)
{
	struct clk_imx6pll *c = container_of(clk, struct clk_imx6pll, clk);

	iowrite32(c->reg + REG_SET, PLL_POWERDOWN);
}

static unsigned long clk_imx6pll_get_rate(struct clk *clk)
{
	struct clk_imx6pll *c = container_of(clk, struct clk_imx6pll, clk);
	ulong prate = clk_get_rate(clk->parent);
	uint val;
	uint div;

	val = ioread32(c->reg + REG_VAL);
	div = val & c->mask;
	if (div == 1)
		return prate * 22;
	else
		return prate * 20;
}

static const struct clk_ops clk_imx6pll_ops = {
	.enable   = clk_imx6pll_enable,
	.disable  = clk_imx6pll_disable,
	.get_rate = clk_imx6pll_get_rate,
};


static unsigned long clk_imx6pll_get_rate_500MHz(struct clk *clk)
{
	return 500000000;
}

static const struct clk_ops clk_imx6pll_enet_ops = {
	.enable   = clk_imx6pll_enable,
	.disable  = clk_imx6pll_disable,
	.get_rate = clk_imx6pll_get_rate_500MHz,
};

int clk_imx6pll_register(struct clk_imx6pll *c, void __iomem *ccmbase, uint flags)
{
	const struct clk_ops *ops;
	uint type = flags & 0x7;
	u32 mask = 0;
	ulong reg;

	switch (type) {
	case IMX6PLLT_SYS:
		ops = &clk_imx6pll_ops;
		reg = CCM_PLL_SYS;
		mask = 0x1;
		break;
	case IMX6PLLT_ENET:
		ops = &clk_imx6pll_enet_ops;
		reg = CCM_PLL_ENET;
		break;
	default:
		return -EINVAL;
	}

	c->flags = flags;
	c->reg = ccmbase + reg;
	c->mask = mask;

	// use the 24MHz ref clock as bypass src & do not bypass
	iowrite32(c->reg + REG_CLR, PLL_BYPASS|PLL_BYPASS_SRC_MSK);

	return clk_register(&c->clk, ops);
}
