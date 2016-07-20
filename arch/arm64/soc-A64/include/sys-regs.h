#ifndef _SOC_SYS_REGS_H_
#define _SOC_SYS_REGS_H_

#define	CPUCFG_BASE	0x01700000
#define	CPUCFG_SIZE	0x00000400

#define CPUCFG_CLX_CTRL0	0x00	// Cluster Control
#define CPUCFG_CLX_CTRL0_AARCH64(CPU)	(1 << (24 + (CPU)))

#define CPUCFG_CLX_CTRL1	0x04
#define CPUCFG_CACHE_CFG0	0x08	// Cache parameters config
#define CPUCFG_CACHE_CFG1	0x0C
#define CPUCFG_DBG0		0x20	// not in manual
#define CPUCFG_GENER_CTRL0	0x28	// General Control
#define CPUCFG_CPU_STATUS	0x30	// Cluster CPU Status
#define CPUCFG_L2_STATUS	0x3C	// L2 Status
#define CPUCFG_CLX_RST_CTRL	0x80	// Cluster Reset Control

#define CPUCFG_RVBAR_L(CPU)	(0xA0 + (CPU) * 8 + 0)	// Reset Vector Base Address
#define CPUCFG_RVBAR_H(CPU)	(0xA0 + (CPU) * 8 + 4)


////////////////////////////////////////////////////////////////////////////////

#define	R_CPUCFG_BASE	0x01F01C00
#define	R_CPUCFG_SIZE	0x00000400

#define	R_CPUCFG_CLX_PWR_RST	0x30	// CPU power-on reset

////////////////////////////////////////////////////////////////////////////////

#define	PRCM_BASE	0x01F01400
#define	PRCM_SIZE	0x00000400

//#define PRCM_CPUS_RST			0x000	// CPUS Reset
//#define PRCM_CPUS_RST_ASSERT			(0 << 0)
//#define PRCM_CPUS_RST_DEASSERT			(1 << 0)
//#define PRCM_CLX_RST_CTRL(CL)		(0x004	+ (CL) * 4)	// Cluster Reset Control
//#define PRCM_CPUS_CLK_CFG		0x010	// CPUS Clock Configuration
//#define PRCM_APBS_CLK_DIV		0x01C	// APBS Clock Divide
//#define PRCM_APBS_CLK_GATING		0x028	// APBS Clock Gating
//#define PRCM_PLL_CTRL1			0x044	// PLL Control
//#define PRCM_ONE_WIRE_CLK		0x050	// One Wire Clock
//#define PRCM_CIR_RX_CLK			0x054	// CIR_RX Clock
//#define PRCM_DAUDIO0_CLK		0x058	// DAUDIO0 Clock
//#define PRCM_DAUDIO1_CLK		0x05C	// DAUDIO1 Clock
//#define PRCM_CPU_PWR_STATUS(CPUID)	(0x064 + (CPUID) * 4) // not in manual!
//#define PRCM_APBS_SOFT_RST		0x0B0	// APBS Software Reset
#define PRCM_CLX_PWROFF_GATING		0x100	// Cluster Power-off Gating
//#define PRCM_VDD_SYS_PWROFF_GATING	0x110	// VDD_SYS Power Off Gating
//#define PRCM_GPU_PWROFF_GATING		0x118	// GPU Power Off Gating
//#define PRCM_VDD_SYS_RST		0x120	// VDD_SYS Reset
#define PRCM_CPU_PWR_SWITCH(CPU)	(0x140 + (CPU) * 4)	// CPU Power Clamp
//#define PRCM_SUPER_STB_FLAG		0x160	// Super Standby Flag
//#define PRCM_CPU_SOFT_ENTRY		0x164	// CPU Software Entry
//#define PRCM_SUPER_STB_SOFT_ENTRY	0x168	// Super Standby Software Entry
//#define PRCM_CHIP_VERSION		0x190	// not in manual!
//#define PRCM_NMI_IRQ_CTRL		0x1A0	// NMI IRQ Control
//#define PRCM_NMI_IRQ_EN			0x1A4	// NMI IRQ Enable
//#define PRCM_NMI_IRQ_STATUS		0x1A8	// NMI IRQ Status
//#define PRCM_PLL_AUDIO_CTRL		0x1C0	// PLL_AUDIO Control
//#define PRCM_PLL_AUDIO_BIAS		0x1C4	// PLL_AUDIO Bias
//#define PRCM_PLL_AUDIO_PAT_CFG		0x1C8	// PLL_AUDIO Pattern Control
//#define PRCM_PLL_AUDIO_CTRL_SWITCH	0x1CC	// AUDIO_PLL Control Switch
//#define PRCM_PIO_HOLD_CTRL		0x1F0	// R_PIO Hold Control
//#define PRCM_OSC24M_CTRL		0x1F4	// OSC24M Control

#endif
