#ifndef _ARM_GIC_H_
#define _ARM_GIC_H_


// Distributor
#define GICD_CTLR		 0x0000
#define GICD_CTLR_ENABLE		0x1
#define GICD_CTLR_DISABLE		0x0
#define GICD_TYPER		 0x0004
#define	GICD_TYPER_LINES(VAL)		((VAL) & 0x1f)
#define GICD_IIDR		 0x0008
#define GICD_IGROUP(N)		(0x0080 + (N) * 4)
#define GICD_ISENABLER(N)	(0x0100 + (N) * 4)
#define GICD_ICENABLER(N)	(0x0180 + (N) * 4)
#define GICD_ISPENDINGR(N)	(0x0200 + (N) * 4)
#define GICD_ICPENDINGR(N)	(0x0280 + (N) * 4)
#define GICD_ISACTIVER(N)	(0x0300 + (N) * 4)
#define GICD_ICACTIVER(N)	(0x0380 + (N) * 4)
#define GICD_IPRIOR(N)		(0x0400 + (N) * 4)
#define GICD_ITARGETSR(N)	(0x0800 + (N) * 4)
#define GICD_ICFGR(N)		(0x0c00 + (N) * 4)
#define GICD_ICFGR_LEVEL		0b00
#define GICD_ICFGR_EDGE			0b01
#define GICD_ICFGR_MASK			0b11
#define GICD_NSACR(N)		(0x0e00 + (N) * 4)
#define GICD_SGIR		 0x0f00
#define	GICD_SGIR_DEST_LIST		(0 << 24)
#define	GICD_SGIR_DEST_OTHER		(1 << 24)
#define	GICD_SGIR_DEST_SELF		(2 << 24)
#define	GICD_SGIR_LIST(CPU)		((1 << (CPU)) << 16)
#define	GICD_SGIR_LIST_ALL		(0xff << 16)
#define	GICD_SGIR_IPI(IPI)		(IPI)
#define GICD_CPENDSGIR(N)	(0x0f10 + (N) * 4)
#define GICD_SPENDSGIR(N)	(0x0f20 + (N) * 4)

//#define GICD_INT_ACTLOW_LVLTRIG		0x0


// CPU
#define GICC_CTLR		 0x0000
#define GICC_CTLR_ENABLE		0x1
#define GICC_PMR		 0x0004
#define GICC_BPR		 0x0008
#define GICC_IAR		 0x000c
#define GICC_IAR_INTID(IAR)		((IAR) & 0x3ff)
#define GICC_IAR_CPUID(IAR)		(((IAR) >> 10 ) & 0x7)
#define GICC_EOIR		 0x0010
#define GICC_RPR		 0x0014
#define GICC_HPPIR		 0x0018
#define GICC_ABPR		 0x001C
#define GICC_AIAR		 0x0020
#define GICC_AEOIR		 0x0024
#define GICC_AHPPIR		 0x0028
#define GICC_APR(N)		(0x00d0 + (N) * 4)
#define GICC_NSAPR(N)		(0x00e0 + (N) * 4)
#define GICC_IIDR		 0x00fc
#define GICC_DIR		 0x1000


#define	GIC_PPI_MASK			0xffff0000
#define	GIC_SGI_MASK			0x0000ffff

#define GIC_PRIO_DEFAULT		0xc0
#define GIC_PRIO_FILTER			0xf0

#define	GIC_MAX_CPU			8

#define	GIC_MAX_IRQ			1020

#endif
