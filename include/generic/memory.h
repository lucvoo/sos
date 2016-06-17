#ifndef	_GENERIC_MEMORY_H_
#define	_GENERIC_MEMORY_H_

#include <types/paddr_t.h>

static inline void *phys_to_virt(paddr_t phys)
{
	return (void *) phys + (VIRT_ADDR - PHYS_ADDR);
}

static inline paddr_t virt_to_phys(const void *virt)
{
	return (paddr_t) virt - (VIRT_ADDR - PHYS_ADDR);
}


#define	__phys_to_pfn(phys)	((uint)(((phys) - PHYS_ADDR) >> PAGE_SHIFT))
#define	__pfn_to_phys(pfn)	((((paddr_t)(pfn)) << PAGE_SHIFT) + PHYS_ADDR)

#endif
