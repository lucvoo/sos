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

// Super Sections
#define	PGD_SSECT_SHIFT		24		// PGD_SECT_SHIFT + 4
#define	PGD_SSECT_SIZE		(0x1 << PGD_SSECT_SHIFT)

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

#define	PTE_SPAGE_TEX(x)	((x) <<  6)	// memory attributes
#define	PTE_SPAGE_XN		(0x1 <<  1)	// eXecute Never

// Second level Large pages (64K)
#define	PTE_LPAGE_SIZE		(0x1 << 16)

#define	PTE_LPAGE_TEX(x)	((x) << 12)	// memory attributes
#define	PTE_LPAGE_XN		(0x1 << 15)	// eXecute Never

#endif
