#ifndef	_CLK_GATE_MMIO_H_
#define	_CLK_GATE_MMIO_H_

/*
 * struct clk's gate operation:
 *	The gating is done via a single bit in a single MM register
 *	Negative logic is supported.
 */

#include "clk-core.h"
#include <iomem.h>


struct clk_gate_mmio {
	void __iomem	*reg;
	unsigned char	bit;
	unsigned char	neg_logic;
};

#endif
