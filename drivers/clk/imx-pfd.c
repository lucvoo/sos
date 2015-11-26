#include "imx-pfd.h"
#include <round.h>
#include <errno.h>
#include <bits.h>
#include <io.h>


#define	REG_VAL		0x0
#define	REG_SET		0x4
#define	REG_CLR		0x8
#define	REG_XOR		0xC

#define	PFD_SHIFT(N)	((N) * 8)
#define	PFD_GATE(N)	(0x80 << PFD_SHIFT(N))
#define	PFD_FRAC	(0x3f)


static int clk_imxpfd_enable(struct clk *clk)
{
	struct clk_imxpfd *c = container_of(clk, struct clk_imxpfd, clk);

	iowrite32(c->reg + REG_CLR, PFD_GATE(c->pfd));
	return 0;
}

static void clk_imxpfd_disable(struct clk *clk)
{
	struct clk_imxpfd *c = container_of(clk, struct clk_imxpfd, clk);

	iowrite32(c->reg + REG_SET, PFD_GATE(c->pfd));
}

static unsigned long clk_imxpfd_get_rate(struct clk *clk)
{
	struct clk_imxpfd *c = container_of(clk, struct clk_imxpfd, clk);
	u64 prate = clk_get_rate(clk->parent);
	uint frac;

	frac = (ioread32(c->reg) >> PFD_SHIFT(c->pfd)) & PFD_FRAC;
	return (prate * 18) / frac;
}

static int clk_imxpfd_set_rate(struct clk *clk, ulong target)
{
	struct clk_imxpfd *c = container_of(clk, struct clk_imxpfd, clk);
	ulong prate = clk_get_rate(clk->parent);
	uint frac;

	frac = (prate * 18 + target/2)  / target;
	if (frac < 12)
		frac = 12;
	if (frac > 35)
		frac = 35;

	iowrite32(c->reg + REG_CLR, PFD_FRAC << PFD_SHIFT(c->pfd));
	iowrite32(c->reg + REG_SET,     frac << PFD_SHIFT(c->pfd));
	return 0;
}


static const struct clk_ops clk_imxpfd_ops = {
	.enable   = clk_imxpfd_enable,
	.disable  = clk_imxpfd_disable,
	.get_rate = clk_imxpfd_get_rate,
	.set_rate = clk_imxpfd_set_rate,
};

int clk_imxpfd_register(struct clk_imxpfd *c)
{
	return clk_register(&c->clk, &clk_imxpfd_ops);
}
