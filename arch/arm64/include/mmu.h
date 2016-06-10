#ifndef	_ARCH_MMU_H_
#define	_ARCH_MMU_H_

#include <arch/memory.h>
#include <arch/page.h>

#define	MMU_BITS	(VIRT_BITS-PAGE_BITS)			// 27
#define	MMU_GRAN	(PAGE_BITS-3)				// 9

#define	MMU_LEVEL	((MMU_BITS + MMU_GRAN-1) / MMU_GRAN)	// 3
#define	MMU_TOP_LEVEL	(4 - MMU_LEVEL)

// PGD == top level
#define	PGD_BITS	(MMU_BITS % MMU_GRAN)
#define	PGD_NBR_ENT	(1 << PGD_BITS)

#endif
