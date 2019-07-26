#ifndef _SOC_CPUCFG_H_
#define _SOC_CPUCFG_H_

#include <soc/iobase.h>

#define	CPUCFG_SIZE	0x00000400

// H3
#define CPUCFG_CPUS_RST_CTRL	0x00		// CPUS Reset Control
#define CPUCFG_CPUX_RST(X)	(0x40+(X)*0x40)	// CPUx Reset Control
#define CPUCFG_CPUX_CTRL(X)	(0x44+(X)*0x40)	// CPUx Control
#define CPUCFG_CPUX_STATUS(X)	(0x48+(X)*0x40)	// CPUx Status
#define CPUCFG_CPUX_PWRCLAMP(X)	(0x64+(X)*0x40)	// CPUx Power Clamp Status

#define CPUCFG_CPU_SYS_RST	0x140		// CPU System Reset
#define CPUCFG_CPU_CLK_GATING	0x144		// CPU Clock Gating
#define CPUCFG_GENER_CTRL	0x184		// General Control
#define CPUCFG_SUP_STAN_FLAG	0x1A0		// Super Standby Flag
#define CPUCFG_PRIVATE0		0x1A4
#define CPUCFG_PRIVATE1		0x1A8
#define CPUCFG_DBG_CTL0		0x1E0
#define CPUCFG_DBG_CTL1		0x1E4

#define CPUCFG_CNT64_CTRL	0x280		// 64-bit Counter Control
#define CPUCFG_CNT64_LOW	0x284		// 64-bit Counter Low
#define CPUCFG_CNT64_HIGH	0x288		// 64-bit Counter High

#endif
