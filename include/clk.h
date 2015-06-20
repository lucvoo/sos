#ifndef	_CLK_H_
#define	_CLK_H_

struct clk;

struct clk *clk_get(const char *name);
void clk_put(struct clk *clk);

int clk_enable(struct clk *clk);
void clk_disable(struct clk *clk);

unsigned long clk_get_rate(struct clk *clk);

#endif
