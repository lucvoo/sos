#ifndef	_SOC_IRQ_H_
#define	_SOC_IRQ_H_


#define	GIC_NBR_IRQ	(32 + 180)

#define	GICD_BASE	0x01c41000	// 0x1000
#define	GICC_BASE	0x01c42000	// 0x1000

#define	GICH_BASE	0x01c44000	// 0x2000
#define	GICV_BASE	0x01c46000	// 0x2000

#endif
