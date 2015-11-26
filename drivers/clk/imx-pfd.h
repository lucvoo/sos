#ifndef	_CLK_IMX_PFD_H_
#define	_CLK_IMX_PFD_H_

#include "clk-core.h"
#include <iomem.h>

/*
 * i.MX's Phase Fractional Divider
 */

struct clk_imxpfd {
	struct clk	clk;
	void __iomem	*reg;
	u8		pfd;
};


int clk_imxpfd_register(struct clk_imxpfd *c);

#endif
