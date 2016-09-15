#ifndef	_ARCH_GIC_H_
#define _ARCH_GIC_H_

#include <bits.h>

#define GIC_SH_OFFSET		0x00000
#define GIC_VP_OFFSET		0x08000

#define	GIC_PIN_OFF		2		// FIXME: not valid in VEIC mode


#define GIC_SH_CFG		0x0000		// Config
#define GIC_SH_CFG_COUNTSTOP		BIT(28)
#define GIC_SH_CFG_COUNTBITS(X)		(BITS_EXTRACT(X, BITS(24, 4)) * 4 + 32)
#define GIC_SH_CFG_NBRIRQS(X)		(BITS_EXTRACT(X, BITS(16, 8)) * 8 + 8)
#define GIC_SH_CFG_NBRVPES(X)		(BITS_EXTRACT(X, BITS(0, 8)) + 1)

#define GIC_SH_CNTLo		0x0010		// CounterLo
#define GIC_SH_CNTHi		0x0014		// CounterHi
#define GIC_SH_REVID		0x0020		// Revision ID

#define GIC_SH_HIGH(X)		(0x0100	+ 4 * (X))	// Polarity level low/high
#define GIC_SH_RAISING(X)	(0x0100	+ 4 * (X))	// Polarity edge falling/raising
#define GIC_SH_EDGE(X)		(0x0180	+ 4 * (X))	// Trigger type level/edge
#define GIC_SH_DUAL(X)		(0x0200	+ 4 * (X))	// Dual Edges

#define GIC_SH_WEDGE		0x0280		// Write Edge
#define GIC_SH_WEDGE_SET		(1 << 31)
#define GIC_SH_WEDGE_CLR		(0 << 31)

#define GIC_SH_RMASK(X)		(0x0300	+ 4 * (X))	// Reset Mask (WO)
#define GIC_SH_SMASK(X)		(0x0380	+ 4 * (X))	// Set Mask (WO)
#define GIC_SH_MASK(X)		(0x0400	+ 4 * (X))	// Mask (RO)
#define GIC_SH_PEND(X)		(0x0480	+ 4 * (X))	// Pending (RO)
#define GIC_SH_MPIN(X)		(0x0500	+ 4 * (X))	// Map IRQ to PIN or NMI
#define GIC_SH_MPIN_PIN(X)		((1 << 31) | ((X) - GIC_PIN_OFF))
#define GIC_SH_MPIN_NMI			((1 << 30) | (0))
#define GIC_SH_MPIN_YQ(X)		((1 << 29) | (X))

#define GIC_SH_VPE(X)		(0x2000	+ 0x20 * (X))	// Map IRQ to VPE

#define GIC_SH_SEND_DINT	0x6000		// Send Debug IRQ to VPE



#define GIC_VP_CTRL		0x0000		// Local Interrupt Control
#define GIC_VP_CTRL_FDC			(1 << 4)
#define GIC_VP_CTRL_SWINT		(1 << 3)
#define GIC_VP_CTRL_PERF		(1 << 2)
#define GIC_VP_CTRL_TIMER		(1 << 1)
#define GIC_VP_CTRL_EIC_MODE		(1 << 0)

#define GIC_VP_PEND		0x0004		// Local IRQ Pending
#define GIC_VP_MASK		0x0008		// Local IRQ Mask
#define GIC_VP_RMASK		0x000c		// Local IRQ Clear Mask
#define GIC_VP_SMASK		0x0010		// Local IRQ Set Mask
#define GIC_VP_MASK_FDC			(1 << 6)
#define GIC_VP_MASK_SW1			(1 << 5)
#define GIC_VP_MASK_SW0			(1 << 4)
#define GIC_VP_MASK_PCI			(1 << 3)
#define GIC_VP_MASK_TIMER		(1 << 2)
#define GIC_VP_MASK_COMP		(1 << 1)
#define GIC_VP_MASK_WD			(1 << 0)

#define GIC_VP_WD_MAP		0x0040		// Local WatchDog Map-to-Pin
#define GIC_VP_COMP_MAP		0x0044		// Local GIC Compare Map-to-Pin
#define GIC_VP_TIMER_MAP	0x0048		// Local CPU Timer Map-to-Pin
#define GIC_VP_FDC_MAP		0x004c		// Local CPU Fast Debug Channel Map-to-Pin
#define GIC_VP_PCI_MAP		0x0050		// Local Perf Counter Map-to-Pin
#define GIC_VP_SW0_MAP		0x0054		// Local SWInt0 Map-to-Pin
#define GIC_VP_SW1_MAP		0x0058		// Local SWInt1 Map-to-Pin
#define GIC_VP_MPIN_PIN(X)		((1 << 31) | ((X) - GIC_PIN_OFF))
#define GIC_VP_MPIN_NMI			((1 << 30) | (0))
#define GIC_VP_MPIN_YQ(X)		((1 << 29) | (X))

#define GIC_VP_OTHER_ADDR	0x0080		// VPE-Other Addressing
#define GIC_VP_IDENT		0x0088		// VPE-Local Identification

#define GIC_VP_WD_CFG		0x0090		// Watchdog Timer Config
#define GIC_VP_WD_CFG_RESET		(1 << 7)
#define GIC_VP_WD_CFG_INTR		(1 << 6)
#define GIC_VP_WD_CFG_WAIT		(1 << 5)
#define GIC_VP_WD_CFG_DEBUG		(1 << 4)
#define GIC_VP_WD_CFG_TYPE_ONE_TRIP	(0 << 1)
#define GIC_VP_WD_CFG_TYPE_COUNTDOWN	(1 << 1)
#define GIC_VP_WD_CFG_TYPE_PIT		(2 << 1)
#define GIC_VP_WD_CFG_START		(1 << 0)

#define GIC_VP_WD_CNT		0x0094		// Watchdog Timer Count
#define GIC_VP_WD_INIT		0x0098		// Watchdog Timer Initial Count

#define GIC_VP_CMPLo		0x00A0		// CompareLo
#define GIC_VP_CMPHi		0x00A4		// CompareHi

#define GIC_VP_EICSS(X)	(0x0100 + (X) * 4)	// EIC Shadow Set for IRQ X

#define GIC_VL_DINT_PART	0x3000		// VPE-Local DINT Group Participate
#define GIC_VL_BRK_GROUP	0x3080		// VPE-Local DebugBreak Group

#endif
