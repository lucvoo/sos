#ifndef	_SOC_REGS_COPRO_H_
#define	_SOC_REGS_COPRO_H_

#include <arch/regs-copro.h>

// JZ4780
#define	c0_core_ctrl	__COPRO_REG($12, 2)
#define	CORE_CTRL_SW_RST0	(1 <<  0)
#define	CORE_CTRL_SW_RST1	(1 <<  1)
#define	CORE_CTRL_RPC0		(1 <<  8)
#define	CORE_CTRL_RPC1		(1 <<  9)
#define	CORE_CTRL_SLEEP0	(1 << 16)
#define	CORE_CTRL_SLEEP1	(1 << 17)

#define	c0_core_stat	__COPRO_REG($12, 3)
#define	CORE_STATUS_MIRQ0	(1 <<  0)
#define	CORE_STATUS_MIRQ1	(1 <<  1)
#define	CORE_STATUS_IRQ0	(1 <<  8)
#define	CORE_STATUS_IRQ1	(1 <<  9)
#define	CORE_STATUS_SLEEP0	(1 << 16)
#define	CORE_STATUS_SLEEP1	(1 << 17)

#define	c0_core_reim	__COPRO_REG($12, 4)
#define	CORE_REIM_MIRQ0		(1 <<  0)
#define	CORE_REIM_MIRQ1		(1 <<  1)
#define	CORE_REIM_IRQ0		(1 <<  8)
#define	CORE_REIM_IRQ1		(1 <<  9)
#define	CORE_REIM_ENTRY_MSK	0xFFFF0000

#endif
