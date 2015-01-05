#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>
#include "arch/mach/memory.h"

#if defined(CONFIG_PHYS_ADDR) && !defined(PHYS_ADDR)
#define	PHYS_ADDR	UL(CONFIG_PHYS_ADDR)
#endif

#ifndef	VIRT_ADDR
#define	VIRT_ADDR	UL(CONFIG_VIRT_ADDR)		// Virtual RAM. Same as Linux's PAGE_OFFSET
#endif
#ifndef	TEXT_OFFSET
#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)
#endif


#ifndef	__ASSEMBLY__

#include <types/paddr_t.h>

#ifdef PHYS_ADDR
static inline void *phys_to_virt(paddr_t phys)
{
	return (void *) phys + (VIRT_ADDR - PHYS_ADDR);
}

static inline paddr_t virt_to_phys(const void *virt)
{
	return (paddr_t) virt - (VIRT_ADDR - PHYS_ADDR);
}
#endif

#define	__phys_to_pfn(phys)	((unsigned long) ((phys) >> PAGE_SHIFT))
#define	__pfn_to_phys(pfn)	(((paddr_t)(pfn)) << PAGE_SHIFT)

#endif

#endif
