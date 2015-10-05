#ifndef	_PAGE_H_
#define	_PAGE_H_

#include <asm-ul.h>
#include <arch/page.h>

#define	PAGE_SIZE	(UL(1) << PAGE_SHIFT)
#define	PAGE_MASK	(~(PAGE_SIZE-1))
#define	PAGE_ALIGN(adr)	(((adr)+PAGE_SIZE-1)&PAGE_MASK)

#ifndef	__ASSEMBLY__

#include <dlist.h>

enum page_flags {
	PG_reserved	= 0x00000001,	// reserved for the kernel or ...
	PG_free		= 0x00000002,	// free in the binary buddy
	PG_slab		= 0x00000004,	// used for slab allocation
	PG_perm		= 0x00000008,	// permanent allocation/won't be freed
	PG_kmalloc	= 0x00000010,	// used for non-slab based kmalloc
};


struct page {
	unsigned long	flags;

	unsigned int	order;		// order in the binary buddy (when PG_free)

	struct dlist	list;
} __aligned(32);


// WARNING: most of the time, changing and/or testing the flags need to be done atomically

#endif

#endif
