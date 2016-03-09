#ifndef	_ARCH_IOMAP_H_
#define	_ARCH_IOMAP_H_

#include <arch/memory.h>
#include <iomem.h>


static inline void __iomem * __ioremap(paddr_t phys)
{
	return (void __iomem *) (KSEG1_BASE + phys);
}

#endif
