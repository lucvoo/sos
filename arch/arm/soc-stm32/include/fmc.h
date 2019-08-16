#ifndef _SOC_FMC_H_
#define _SOC_FMC_H_

#define FMC_BASE	0xA0000000
#define FMC_SIZE	    0x1000


#define FMC_BCR1	0x000
#define FMC_BTR1	0x004
#define FMC_BCR2	0x008
#define FMC_BTR2	0x00C
#define FMC_BCR3	0x010
#define FMC_BTR3	0x014
#define FMC_BCR4	0x018
#define FMC_BTR4	0x01C
#define FMC_PCR2	0x060
#define FMC_SR2		0x064
#define FMC_PMEM2	0x068
#define FMC_PATT2	0x06C
#define FMC_ECCR2	0x074
#define FMC_PCR3	0x080
#define FMC_SR3		0x084
#define FMC_PMEM3	0x088
#define FMC_PATT3	0x08C
#define FMC_ECCR3	0x094
#define FMC_PCR4	0x0A0
#define FMC_SR4		0x0A4
#define FMC_PMEM4	0x0A8
#define FMC_PATT4	0x0AC
#define FMC_PIO4	0x0B0
#define FMC_BWTR1	0x104
#define FMC_BWTR2	0x10C
#define FMC_BWTR3	0x114
#define FMC_BWTR4	0x11C
#define FMC_SDCR1	0x140
#define FMC_SDCR2	0x144
#define  FMC_SDCR_NC(X)		(((X)-8) << 0)	// Number of col address bits
#define  FMC_SDCR_NR(X)		(((X)-11)<< 2)	// Number of row address bits
#define  FMC_SDCR_MWID(X)	(((X)/16) <<  4)// Memory data bus width
#define  FMC_SDCR_NB(X)		(((X)/4) <<  6)	// Number of internal banks
#define  FMC_SDCR_CAS(X)	((X) <<  7)	// CAS latency
#define  FMC_SDCR_WP		(  1 <<  9)	// Write protection
#define  FMC_SDCR_SDCLK(X)	((X) << 10)	// SDRAM clock configuration
#define  FMC_SDCR_RBURST	(  1 << 12)
#define  FMC_SDCR_RPIPE(X)	((X) << 13)
#define FMC_SDTR1	0x148
#define FMC_SDTR2	0x14C
#define  FMC_SDTR_TMRD(X)	((X) << 0)	// Load mode register to active
#define  FMC_SDTR_TXSR(X)	((X) << 4)	// Exit self-refresh time
#define  FMC_SDTR_TRAS(X)	((X) << 8)	// Self-refresh time
#define  FMC_SDTR_TRC(X)	((X) << 12)	// Row cycle delay
#define  FMC_SDTR_TWR(X)	((X) << 16)	// Recovery delay
#define  FMC_SDTR_TRP(X)	((X) << 20)	// Row precharge delay
#define  FMC_SDTR_TRCD(X)	((X) << 24)	// Row-to-column delay
#define FMC_SDCMR	0x150
#define  FMC_SDCMR_MODE_NORMAL		0
#define  FMC_SDCMR_MODE_CLOCK_CFG_EN	1
#define  FMC_SDCMR_MODE_PALL		2
#define  FMC_SDCMR_MODE_AUTOREFRESH	3
#define  FMC_SDCMR_MODE_LOAD_MODE	4
#define  FMC_SDCMR_MODE_SELFREFRESH	5
#define  FMC_SDCMR_MODE_POWERDOWN	6
#define  FMC_SDCMR_CTB2		(1 << 3)
#define  FMC_SDCMR_CTB1		(1 << 4)
#define  FMC_SDCMR_NRFS(X)	((X) << 5)	// Number of auto-refresh cycle
#define  FMC_SDCMR_MRD(X)	((X) << 9)
#define FMC_SDRTR	0x154			// Refresh Timer Register
	// Refresh rate = (SDRAM refresh rate * SDRAM clock frequency) - 20
	// SDRAM refresh rate = SDRAM refresh period / Number of rows
#define  FMC_SDRTR_CRE		(  1 <<  0)
#define  FMC_SDRTR_COUNT(X)	((X) <<  1)	// Refresh rate
#define  FMC_SDRTR_REIE		(  1 << 14)
#define FMC_SDSR	0x158
#define  FMC_SDSR_RE			(1 << 0)
#define  FMC_SDSR_BUSY			(1 << 5)

#endif
