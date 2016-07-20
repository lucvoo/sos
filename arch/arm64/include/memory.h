#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>
#include <arch/page.h>
#include <soc/memory.h>


#define	VIRT_BITS	CONFIG_VA_BITS
#define	VIRT_LAST	UL(0xffffffffffffffff)

#define	PHYS_ADDR	UL(CONFIG_PHYS_ADDR)
#ifdef	CONFIG_NOMMU
#define	VIRT_ADDR	UL(CONFIG_VIRT_ADDR)
#define	IOMEM_VIRT	IOMEM_PHYS
#else
#define	VIRT_ADDR	(VIRT_LAST << VIRT_BITS)
#define	IOMEM_VIRT	(VIRT_LAST - IOMEM_SIZE + 1)
#endif

#ifndef	TEXT_OFFSET
#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)
#endif



// TODO: reserve space for VMALLOC, PCI IO, ...


#ifndef	__ASSEMBLY__

#include <generic/memory.h>

#endif

#endif
