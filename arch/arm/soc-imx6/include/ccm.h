#ifndef	_SOC_CCM_H_
#define	_SOC_CCM_H_

#include <bits.h>


#define	CCM_BASE	0x020C4000
#define	CCM_SIZE	    0x8000	// CCM + ANALOG


#define	CCM_CCR		0x00	// Control Register
#define		CCM_CCR_OSCNT	BITS(0, 7)
#define		CCM_CCR_COSC_EN	BIT(12)
#define		CCM_CCR_RBCNT	BITS(21, 6)
#define		CCM_CCR_RBC_EN	BIT(27)
#define	CCM_CCDR	0x04	// Control Divider Register
#define	CCM_CSR		0x08	// Status Register
#define	CCM_CCSR	0x0C	// Clock Switcher Register
#define	CCM_CACRR	0x10	// Arm Clock Root Register
#define	CCM_CBCDR	0x14	// Bus Clock Divider Register
#define		CBCDR_PERIPH2_CLK2_PODF(D)	(((D) - 1) << 0)
#define		CBCDR_IPG_PODF(D)		(((D) - 1) << 8)
#define		CBCDR_AHB_PODF(D)		(((D) - 1) << 10)
#define		CBCDR_PERIPH_CLK_SEL		BIT(25)
#define		CBCDR_PERIPH2_CLK_SEL		BIT(26)
#define		CBCDR_PERIPH_CLK2_PODF(D)	(((D) - 1) << 27)

#define	CCM_CBCMR	0x18	// Bus Clock Multiplexer Register
#define	CCM_CSCMR1	0x1C	// Serial Clock Multiplexer Register 1
#define	CCM_CSCMR2	0x20	// Serial Clock Multiplexer Register 2
#define	CCM_CSCDR1	0x24	// Serial Clock Divider Register 1
#define	CCM_CS1CDR	0x28	// SSI1 Clock Divider Register
#define	CCM_CS2CDR	0x2C	// SSI2 Clock Divider Register
#define	CCM_CDCDR	0x30	// D1 Clock Divider Register
#define	CCM_CHSCCDR	0x34	// HSC Clock Divider Register
#define	CCM_CSCDR2	0x38	// Serial Clock Divider Register 2
#define	CCM_CSCDR3	0x3C	// Serial Clock Divider Register 3
#define	CCM_CDHIPR	0x48	// Divider Handshake In-Process Register
#define	CCM_CLPCR	0x54	// Low Power Control Register
#define	CCM_CISR	0x58	// Interrupt Status Register
#define	CCM_CIMR	0x5C	// Interrupt Mask Register
#define	CCM_CCOSR	0x60	// Clock Output Source Register
#define	CCM_CGPR	0x64	// General Purpose Register
#define	CCM_CCGR0	0x68	// Clock Gating Register 0
#define	CCM_CCGR1	0x6C	// Clock Gating Register 1
#define	CCM_CCGR2	0x70	// Clock Gating Register 2
#define	CCM_CCGR3	0x74	// Clock Gating Register 3
#define	CCM_CCGR4	0x78	// Clock Gating Register 4
#define	CCM_CCGR5	0x7C	// Clock Gating Register 5
#define	CCM_CCGR6	0x80	// Clock Gating Register 6
#define		GATE_OFF	0x0
#define		GATE_RUN	0x1
#define		GATE_WAIT	0x3
#define		CCM_CG(N, G)	((G) << (N*2))

#define	CCM_CMEOR	0x88	// Module Enable Overide Register


#define	CCM_ANALOG	0x4000
#define	CCM_PLL_ARM	(CCM_ANALOG + 0x0000)
#define	CCM_PLL_USB1	(CCM_ANALOG + 0x0010)
#define	CCM_PLL_USB2	(CCM_ANALOG + 0x0020)
#define	CCM_PLL_SYS	(CCM_ANALOG + 0x0030)
#define	CCM_PLL_AUDIO	(CCM_ANALOG + 0x0070)
#define	CCM_PLL_VIDEO	(CCM_ANALOG + 0x00A0)
#define	CCM_PLL_ENET	(CCM_ANALOG + 0x00E0)
#define	    PLL_ENET_125M_REF	(1 << 13)
#define	    PLL_ENET_25M_REF	(1 << 21)
#define	    PLL_POWERDOWN	(1 << 12)
#define	    PLL_LOCKED		(1 << 31)
#define	    PLL_BYPASS		(1 << 16)
#define	    PLL_BYPASS_SRC_MSK	0x0000c000
#define	CCM_PLL_SYS_PFD	(CCM_ANALOG + 0x0100)

#endif
