#ifndef	_SOC_PRCM_H_
#define	_SOC_PRCM_H_

#define	PRCM_BASE	0x08001400
#define	PRCM_SIZE	0x00000400

#define PRCM_CPUS_RST			0x000	// CPUS Reset
#define PRCM_CPUS_RST_ASSERT			(0 << 0)
#define PRCM_CPUS_RST_DEASSERT			(1 << 0)
#define PRCM_CX_RST_CTRL(CL)		(0x004	+ (CL) * 4)	// Cluster Reset Control
#define PRCM_CPUS_CLK_CFG		0x010	// CPUS Clock Configuration
#define PRCM_APBS_CLK_DIV		0x01C	// APBS Clock Divide
#define PRCM_APBS_CLK_GATING		0x028	// APBS Clock Gating
#define PRCM_PLL_CTRL1			0x044	// PLL Control
#define PRCM_ONE_WIRE_CLK		0x050	// One Wire Clock
#define PRCM_CIR_RX_CLK			0x054	// CIR_RX Clock
#define PRCM_DAUDIO0_CLK		0x058	// DAUDIO0 Clock
#define PRCM_DAUDIO1_CLK		0x05C	// DAUDIO1 Clock
#define PRCM_CPU_PWR_STATUS(CPUID)	(0x064 + (CPUID) * 4) // not in manual!
#define PRCM_APBS_SOFT_RST		0x0B0	// APBS Software Reset
#define PRCM_CX_PWROFF_GATING(CL)	(0x100+ (CL) * 4)	// Cluster Power-off Gating
#define PRCM_VDD_SYS_PWROFF_GATING	0x110	// VDD_SYS Power Off Gating
#define PRCM_GPU_PWROFF_GATING		0x118	// GPU Power Off Gating
#define PRCM_VDD_SYS_RST		0x120	// VDD_SYS Reset
#define PRCM_CPU_PWR_SWITCH(CPUID)	(0x140 + (CPUID) * 4)	// CPU Power Clamp
#define PRCM_SUPER_STB_FLAG		0x160	// Super Standby Flag
#define PRCM_CPU_SOFT_ENTRY		0x164	// CPU Software Entry
#define PRCM_SUPER_STB_SOFT_ENTRY	0x168	// Super Standby Software Entry
#define PRCM_CHIP_VERSION		0x190	// not in manual!
#define PRCM_NMI_IRQ_CTRL		0x1A0	// NMI IRQ Control
#define PRCM_NMI_IRQ_EN			0x1A4	// NMI IRQ Enable
#define PRCM_NMI_IRQ_STATUS		0x1A8	// NMI IRQ Status
#define PRCM_PLL_AUDIO_CTRL		0x1C0	// PLL_AUDIO Control
#define PRCM_PLL_AUDIO_BIAS		0x1C4	// PLL_AUDIO Bias
#define PRCM_PLL_AUDIO_PAT_CFG		0x1C8	// PLL_AUDIO Pattern Control
#define PRCM_PLL_AUDIO_CTRL_SWITCH	0x1CC	// AUDIO_PLL Control Switch
#define PRCM_PIO_HOLD_CTRL		0x1F0	// R_PIO Hold Control
#define PRCM_OSC24M_CTRL		0x1F4	// OSC24M Control

#endif
