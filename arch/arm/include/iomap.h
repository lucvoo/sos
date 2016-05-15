#ifndef	_ARCH_IOMAP_H_
#define	_ARCH_IOMAP_H_

#include <types/paddr_t.h>
#include <iomem.h>


// NOTE: only to be used for when the MMU:
//	 * is still using the identity mapping
//	 * or is not yet initialized at all
static inline void __iomem * __ioremap(paddr_t phys)
{
	return (void __iomem *) phys;
}

#endif
