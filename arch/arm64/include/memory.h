#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>
#include <arch/page.h>
#include <soc/memory.h>


#define	VIRT_BITS	CONFIG_VA_BITS
#define	VIRT_LAST	UL(0xffffffffffffffff)

#define	PHYS_ADDR	UL(CONFIG_PHYS_ADDR)
#define	VIRT_ADDR	(UL(0xffffffffffffffff) << VIRT_BITS)

#ifndef	TEXT_OFFSET
#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)
#endif


// TOP IOMEM_SIZE
#define	IOMEM_VIRT	(VIRT_LAST - IOMEM_SIZE + 1)

// TODO: reserve space for VMALLOC, PCI IO, ...


#ifndef	__ASSEMBLY__

#include <generic/memory.h>

#endif

#endif
