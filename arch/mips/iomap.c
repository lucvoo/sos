#include <io.h>
#include <arch/iomap.h>


void __iomem *ioremap(paddr_t phys, unsigned long size)
{
	return __ioremap(phys);
}
