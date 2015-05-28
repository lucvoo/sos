#ifndef	_IO_H_
#define	_IO_H_

#include <arch/io.h>
#include <types/paddr_t.h>


void __iomem *ioremap(paddr_t phys, unsigned long size);

static inline void iounmap(void __iomem *addr, unsigned long size)
{
	// FIXME
}


#ifdef CONFIG_NOMMU
static inline void __iomem *__ioremap_nop(paddr_t phys, unsigned long size)
{
	return (void __iomem *) phys;
}

#define	ioremap(A, S)	__ioremap_nop(A, S)
#endif

#endif
