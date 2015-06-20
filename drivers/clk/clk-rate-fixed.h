#ifndef	_CLK_RATE_FIXED_H_
#define	_CLK_RATE_FIXED_H_

/*
 * struct clk's rate operation:
 *	The rate is fixed.
 */

#include "clk-core.h"


struct clk_rate_fixed {
	unsigned long	rate;
	unsigned long	accuracy;
};

#endif
