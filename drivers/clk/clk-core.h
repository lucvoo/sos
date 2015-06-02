#ifndef	_CLK_CORE_H_
#define	_CLK_CORE_H_

#include <clk.h>
#include <lock.h>


struct clk_ops {
	int		(*enable)(struct clk *clk);
	void		(*disable)(struct clk *clk);
};

struct clk {
	struct lock	lock;
	struct clk_ops	*ops;
	int		ena_cnt;
};

#endif
