#ifndef	_ARCH_MMU_HW_H_
#define	_ARCH_MMU_HW_H_

#include <bits.h>


#define	TCR_TG_4K	0b00
#define	TCR_TG_16K	0b01
#define	TCR_TG_64K	0b10
#define	TCR_TG		((CONFIG_PAGE_BITS - 12) / 2)

#define	TCR_SH_NS	0b00
#define	TCR_SH_OS	0b10
#define	TCR_SH_IS	0b11

#define	TCR_RGN_NC	0b00
#define	TCR_RGN_WBWA	0b01
#define	TCR_RGN_WT	0b10
#define	TCR_RGN_WBnWA	0b11

#define	TCR_HD		(1 << 39)
#define	TCR_HA		(1 << 38)
#define	TCR_TBI1	(1 << 38)
#define	TCR_TBI0	(1 << 37)
#define	TCR_AS16	(1 << 36)
#define	TCR_IPS(X)	((X) << 32)
#define	TCR_TG1(X)	((X) << 30)
#define	TCR_SH1(X)	((X) << 28)
#define	TCR_ORGN1(X)	((X) << 26)
#define	TCR_IRGN1(X)	((X) << 24)
#define	TCR_RGN1(X)	(TCR_ORGN1(X)|TCR_IRGN1(X))
#define	TCR_EPD1	(1 << 23)
#define	TCR_A1		(1 << 22)
#define	TCR_T1SZ(X)	((X) << 16)
#define	TCR_TG0(X)	((X) << 14)
#define	TCR_SH0(X)	((X) << 12)
#define	TCR_ORGN0(X)	((X) << 10)
#define	TCR_IRGN0(X)	((X) << 8)
#define	TCR_RGN0(X)	(TCR_ORGN0(X)|TCR_IRGN0(X))
#define	TCR_T0SZ(X)	((X) << 0)


#define	MAIR_TYPE(U,L)	(U << 4|L << 0)
#define	MAIR_DTYPE(A)	MAIR_TYPE(0, A)
#define	MAIR_MTYPE(A)	MAIR_TYPE(A, A)

#define	MAIR_DEV_nGnRnE	MAIR_DTYPE(0b0000)
#define	MAIR_DEV_nGnRE	MAIR_DTYPE(0b0100)
#define	MAIR_DEV_nGRE	MAIR_DTYPE(0b1000)
#define	MAIR_DEV_GRE	MAIR_DTYPE(0b1100)
#define	MAIR_MEM_NC	MAIR_MTYPE(0b0100)
#define	MAIR_MEM_WT	MAIR_MTYPE(0b1011)	// WT, R&W allocate
#define	MAIR_MEM_WB	MAIR_MTYPE(0b1111)	// WB, R&W allocate

#define	MAIR(IDX, ATTR)	(ATTR << (IDX << 3))


/*
 * PTD ~ Page Table Directory = level 0, 1 & 2
 * PTE ~ Page Table Entry     = level 3 page (or level 0, 1, 2 block)
 * PTX: PTD ot PTE
 */
#define	PTX_TYPE_MASK	0b11
#define	PTX_TYPE_INVAL	0b00
#define	PTD_TYPE_BLK	0b01
#define	PTD_TYPE_TBL	0b11
#define	PTE_TYPE_PAG	0b11

// Hierarchical control, unused, PTD_TYPE_TBL only
#define	PTD_NST		(1UL << 63)
#define	PTD_APT		(3UL << 61)
#define	PTD_UXNT	(1UL << 60)
#define	PTD_PXNT	(1UL << 59)

// for PTD_TYPE_BLK & PTE_TYPE_PAG
#define	PTE_UXN		(1UL << 54)
#define	PTE_PXN		(1UL << 53)
#define	PTE_CONT	(1UL << 52)
#define	PTE_DBM		(1UL << 51)		// ARMv8.1 only
#define	PTE_nG		(1UL << 11)
#define	PTE_AF		(1UL << 10)
#define	PTE_NSH		(0UL << 8)
#define	PTE_OSH		(2UL << 8)
#define	PTE_ISH		(3UL << 8)
#define	PTE_RO		(1UL << 7)
#define	PTE_USER	(1UL << 6)
#define	PTE_NS		(1UL << 5)		// ignored if NS state
#define	PTE_MAIDX(IDX)	((IDX) << 2)


#endif
