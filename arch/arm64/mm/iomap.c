#include <iomap.h>
#include <io.h>
#include <arch/memory.h>
#include <soc/memory.h>


// We have plenty of address space
// For now do this stupidly:
// * reserve a big zone for all IO mapping
// * map this zone with a single region at (early) init time
// Cons: no MMU protection


////////////////////////////////////////////////////////////////////////////////

void __iomem *ioremap(paddr_t phys, ulong size)
{
	ulong res;

	res = (phys - IOMEM_PHYS) + IOMEM_VIRT;
	if (phys < IOMEM_PHYS)
		res = 0;
	if ((phys + size) > (IOMEM_PHYS + IOMEM_SIZE))
		res = 0;

	return (void __iomem*)res;
}
