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


static int __clk_enable_locked(struct clk *clk)
{

	if (clk->ena_cnt == 0) {
		int rc = 0;

		rc = clk_enable(clk->parent);
		if (rc)
			return rc;

		if (clk->ops->enable) {
			rc = clk->ops->enable(clk);
			if (rc) {
				clk_disable(clk->parent);
				return rc;
			}
		}
	}

	clk->ena_cnt++;
	return 0;
}

int clk_enable(struct clk *clk)
{
	int rc = 0;

	if (!clk)
		return 0;

	lock_acq(&clk->lock);
	rc = __clk_enable_locked(clk);
	lock_rel(&clk->lock);

	return rc;
}

static void __clk_disable_locked(struct clk *clk)
{
	if (--clk->ena_cnt > 0)
		return;

	if (clk->ops->disable)
		clk->ops->disable(clk);

	clk_disable(clk->parent);
}

void clk_disable(struct clk *clk)
{
	if (!clk)
		return;

	lock_acq(&clk->lock);
	__clk_disable_locked(clk);
	lock_rel(&clk->lock);
}


unsigned long clk_get_rate(struct clk *clk)
{
	if (!clk)
		return 0;

	if (clk->ops->get_rate)
		return clk->ops->get_rate(clk);

	return clk_get_rate(clk->parent);
}

int clk_set_rate(struct clk *clk, ulong rate)
{
	if (!clk)
		return -EINVAL;

	if (!clk->ops->set_rate)
		return -EPERM;

	return clk->ops->set_rate(clk, rate);
}
