#ifndef _SOC_CPUCFG_H_
#define _SOC_CPUCFG_H_

#include <soc/iobase.h>

#define	CPUCFG_SIZE	0x00000400

// A80
#define CPUCFG_CX_CTRL0(CL)	(0x00+(CL)*16)	// Cluster Control
#define CPUCFG_CX_CTRL1(CL)	(0x04+(CL)*16)	// Cluster Control
#define CPUCFG_CX_ADB400(CL)	(0x08+(CL)*16)	// ADB400 powerdown
#define CPUCFG_GENER_CTRL0	0x28		// General Control
#define CPUCFG_GENER_CTRL1	0x2C		// General Control
#define CPUCFG_CX_STATUS(CL)	(0x30+(CL)*4)	// Cluster CPU Status
#define CPUCFG_IRQ_STATUS	0x3C		// Cluster CPU IRQ Status
#define CPUCFG_CX_RESET(CL)	(0x80+(CL)*4)	// Cluster Reset Control
#define CPUCFG_GIC_JTAG_RESET	0x88		// GIC and JTAG Reset Control

#endif
