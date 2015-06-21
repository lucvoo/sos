#ifndef	_CLK_RATE_RATIO_H_
#define	_CLK_RATE_RATIO_H_

/*
 * struct clk's rate operation:
 *	The rate is a fixed rational multiple of its parent.
 */

#include "clk-core.h"


struct clk_rate_ratio {
	unsigned int	mult;
	unsigned int	div;
};

#endif
