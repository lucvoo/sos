#ifndef	_SOC_IRQ_H_
#define	_SOC_IRQ_H_

#include <soc/iobase.h>

#define	GIC_NBR_IRQ	(32 + 180)

#define	GICD_BASE	(GIC_BASE + 0x1000)	// 0x1000
#define	GICC_BASE	(GIC_BASE + 0x2000)	// 0x2000
#define	GICH_BASE	(GIC_BASE + 0x4000)	// 0x2000
#define	GICV_BASE	(GIC_BASE + 0x6000)	// 0x2000

#endif
