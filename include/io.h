#ifndef	_IO_H_
#define	_IO_H_

#include <arch/io.h>

void __iomem *ioremap(unsigned long phys, unsigned long size);

static inline void iounmap(void __iomem *addr, unsigned long size)
{
	// FIXME
}

#endif
