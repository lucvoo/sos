#ifndef	_CLK_IMX6_PLL_H_
#define	_CLK_IMX6_PLL_H_

#include "clk-core.h"
#include <iomem.h>

/*
 * i.MX6's PLL
 */


#define IMX6PLLT_ENET		(6<<0)

#define IMX6PLLF_POWERUP	(1<<8)	// bit set when powered-up not -down


struct clk_imx6pll {
	struct clk	clk;
	void __iomem	*reg;
	uint		flags;
	uint		mask;		// mask for divisor's field
};


int clk_imx6pll_register(struct clk_imx6pll *c, void __iomem *ccmbase, uint flags);

#endif
