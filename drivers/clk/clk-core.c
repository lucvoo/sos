#include "clk-core.h"
#include <string.h>
#include <errno.h>
#include <lock.h>


/******************************************************************************/

static struct {
	struct lock	lock;
	struct clk	*list;
} clks;

static struct clk *clk_lookup(const char *name)
{
	struct clk *clk;

	for (clk = clks.list; clk; clk = clk->next)
		if (streq(name, clk->name))
			break;

	return clk;
}

int clk_register(struct clk *clk, const struct clk_ops *ops)
{
	int rc;

	lock_acq(&clks.lock);
	if (!clk_lookup(clk->name)) {
		clk->ops = ops;
		clk->next = clks.list;
		clks.list = clk;
		rc = 0;
	} else
		rc = -EINVAL;
	lock_rel(&clks.lock);

	return rc;
}

/******************************************************************************/


struct clk *clk_get(const char *name)
{
	struct clk *clk;

	lock_acq(&clks.lock);
	if ((clk = clk_lookup(name))) {
		// FIXME: do we need refcounting?
		// FIXME: what about the parent?
	}
	lock_rel(&clks.lock);

	return clk;
}

void clk_put(struct clk *clk)
{
	// FIXME: do we need refcounting?
	// FIXME: insure we're disabled if refcount reach 0?
	// FIXME: what about the parent?
}


int clk_enable(struct clk *clk)
{
	int rc = 0;

	lock_acq(&clk->lock);
	if (clk->ena_cnt++ == 0) {
		if (clk->ops->enable)
			rc = clk->ops->enable(clk);
	}
	lock_rel(&clk->lock);

	return rc;
}

void clk_disable(struct clk *clk)
{
	lock_acq(&clk->lock);
	if (--clk->ena_cnt == 0) {
		if (clk->ops->disable)
			clk->ops->disable(clk);
	}
	lock_rel(&clk->lock);
}


unsigned long clk_get_rate(struct clk *clk)
{
	if (clk->ops->get_rate)			// FIXME: needed?
		return clk->ops->get_rate(clk);

	return 0;
}
