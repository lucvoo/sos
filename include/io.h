#ifndef	_IO_H_
#define	_IO_H_

#include <arch/io.h>
#include <types/paddr_t.h>

void __iomem *ioremap(paddr_t phys, unsigned long size);

static inline void iounmap(void __iomem *addr, unsigned long size)
{
	// FIXME
}

#endif
