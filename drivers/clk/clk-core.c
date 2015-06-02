#include "clk-core.h"


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
