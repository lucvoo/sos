#ifndef	_ARCH_HW_MMU_H_
#define	_ARCH_HW_MMU_H_


/* Translation Table definitions for:
	- ARMv6
	- ARMv7 with short-descriptor/without LPAE

	cf. ARM ARM B3.5.1
*/

////////////////////////////////////////////////////////////////////////
// Global Directory: ARM Level 1
#define	PGD_NBR_SHIFT		12
#define	PGD_NBR_ENT		(0x1 << PGD_NBR_SHIFT)


#define	PGD_TYPE_MASK		0x00000003
#define	PGD_TYPE_FAULT		0x00000000
#define	PGD_TYPE_L2TBL		0x00000001
#define	PGD_TYPE_SECT		0x00000002

#define	PGD_DOMAIN_MASK		0x000001E0
#define	PGD_DOMAIN(D)		((D) << 5)

#define	PGD_SECT_B		(0x1 <<  2)	// Bufferable
#define	PGD_SECT_C		(0x1 <<  3)	// Cacheable
#define	PGD_SECT_XN		(0x1 <<  4)	// eXecute Never
#define	PGD_SECT_AP0		(0x1 << 10)	// Access Permission
#define	PGD_SECT_AP1		(0x1 << 11)
#define	PGD_SECT_TEX(x)		((x) << 12)	// memory attributes
#define	PGD_SECT_AP2		(0x1 << 15)
#define	PGD_SECT_S		(0x1 << 16)	// Shareable
#define	PGD_SECT_nG		(0x1 << 17)	// not Global
#define	PGD_SECT_SUPER		(0x1 << 18)	// Super section
#define	PGD_SECT_NS		(0x1 << 19)	// Non-Secure

#define	PGD_SECT_AF	TTL1_SECT_AP0		// If SCTLR.AFE == 1

// Sections
#define	PGD_SECT_SHIFT		20		// 32 - PGD_NBR_SHIFT
#define	PGD_SECT_SIZE		(0x1 << PGD_SECT_SHIFT)
#define	PGD_SECT_MASK		(PGD_SECT_SIZE - 1)

// Super Sections
#define	PGD_SSECT_SHIFT		24		// PGD_SECT_SHIFT + 4
#define	PGD_SSECT_SIZE		(0x1 << PGD_SSECT_SHIFT)
#define	PGD_SSECT_MASK		(PGD_SSECT_SIZE - 1)

// Memory type
#define	PGD_MT_MEM	(PGD_SECT_TEX(1)|PGD_SECT_C|PGD_SECT_B)	// normal, WBWA
#define	PGD_MT_DEV	(PGD_SECT_TEX(0)|         0|PGD_SECT_B)	// device shareable

// early init:
#define	PGD_INIT_RW	(PGD_TYPE_SECT|PGD_SECT_AP0)
#define	PGD_INIT_MEM	(PGD_INIT_RW|PGD_MT_MEM)
#define	PGD_INIT_IO	(PGD_INIT_RW|PGD_MT_DEV)

////////////////////////////////////////////////////////////////////////
// Middle Directory: not present
#define	PMD_NBR_SHIFT		0

////////////////////////////////////////////////////////////////////////
// Page Entries: ARM Level 2
#define	PTE_NBR_SHIFT		8
#define	PTE_NBR_ENT		(0x1 << PTE_NBR_SHIFT)

#define	PTE_TYPE_MASK		0x00000003
#define	PTE_TYPE_FAULT		0x00000000
#define	PTE_TYPE_LPAGE		0x00000001
#define	PTE_TYPE_SPAGE		0x00000002

#define	PTE_PAGE_B		(0x1 <<  2)	// Bufferable
#define	PTE_PAGE_C		(0x1 <<  3)	// Cacheable
#define	PTE_PAGE_AP0		(0x1 <<  4)	// Access Permission
#define	PTE_PAGE_AP1		(0x1 <<  5)
#define	PTE_PAGE_AP2		(0x1 <<  9)
#define	PTE_PAGE_S		(0x1 << 10)	// Shareable
#define	PTE_PAGE_nG		(0x1 << 11)	// not Global

#define	PTE_PAGE_AF	PTE_PAGE_AP0		// If SCTLR.AFE == 1

// Second level small pages (4K)
#define	PTE_SPAGE_SIZE		(0x1 << 12)
#define	PTE_SPAGE_MASK		(PTE_SPAGE_SIZE - 1)

#define	PTE_SPAGE_TEX(x)	((x) <<  6)	// memory attributes
#define	PTE_SPAGE_XN		(0x1 <<  1)	// eXecute Never

// Second level Large pages (64K)
#define	PTE_LPAGE_SIZE		(0x1 << 16)

#define	PTE_LPAGE_TEX(x)	((x) << 12)	// memory attributes
#define	PTE_LPAGE_XN		(0x1 << 15)	// eXecute Never

////////////////////////////////////////////////////////////////////////
#define TTBR_C			0x01		// inner-Cacheable (for non-SMP)
#define TTBR_S			0x02		// Shareable
#define TTBR_NOS		0x20		// Not-Outer-Shareable

#define TTBR_ORGN_MASK		0x18		// Regions/outer-cacheability
#define TTBR_ORGN_NC		0x00		// Non-Cacheable
#define TTBR_ORGN_WA		0x08		// Writeback-writeAllocate
#define TTBR_ORGN_WT		0x10		// WriteThrough
#define TTBR_ORGN_WB		0x18		// WriteBack (no WriteAllocate)

#define TTBR_IRGN_MASK		0x41		// Regions/outer-cacheability
#define TTBR_IRGN_NC		0x00		// Non-Cacheable
#define TTBR_IRGN_WA		0x40		// Writeback-writeAllocate
#define TTBR_IRGN_WT		0x01		// WriteThrough
#define TTBR_IRGN_WB		0x41		// WriteBack (no WriteAllocate)


#ifdef CONFIG_SMP
#define	TTBR_FLAGS	(TTBR_IRGN_WA|TTBR_ORGN_WA|TTBR_S|TTBR_NOS)
#else
#define	TTBR_FLAGS	(TTBR_ORGN_WB|TTBR_C)
#endif


// Split between TTBR0/TTBR1
#define	TTBCR_N_NONE		0x0
#define	TTBCR_N_2G2G		0x1
#define	TTBCR_N_1G3G		0x2

#endif
