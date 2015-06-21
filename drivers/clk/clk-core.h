#ifndef	_CLK_CORE_H_
#define	_CLK_CORE_H_

#include <clk.h>
#include <lock.h>


struct clk_ops {
	int		(*enable)(struct clk *clk);
	void		(*disable)(struct clk *clk);

	unsigned long	(*get_rate)(struct clk *clk);
};

struct clk {
	struct lock	lock;
  const struct clk_ops	*ops;
	struct clk	*parent;
	const char	*name;
	int		ena_cnt;

	struct clk	*next;
};


int clk_register(struct clk *clk, const struct clk_ops *ops);

#endif
