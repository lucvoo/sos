#ifndef	_CLK_H_
#define	_CLK_H_

struct clk;

int clk_enable(struct clk *clk);
void clk_disable(struct clk *clk);

#endif
