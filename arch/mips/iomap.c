#include <io.h>
#include <arch/memory.h>


void __iomem *ioremap(paddr_t phys, unsigned long size)
{
	// FIXME
	return (void __iomem *) (KSEG1_BASE + phys);
}
