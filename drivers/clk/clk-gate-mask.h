#ifndef	_CLK_GATE_MASK_H_
#define	_CLK_GATE_MASK_H_

/*
 * struct clk's gate operation:
 *	The gating is done via a bitmask in a single MM register
 */

#include "clk-core.h"
#include <iomem.h>


struct clk_gate_mask {
	void __iomem	*reg;
	u32		msk;
};

#endif
