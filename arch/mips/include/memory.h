#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>


#define	KSEG2_BASE	UL(0xC0000000)
#define	KSEG1_BASE	UL(0xA0000000)
#define	KSEG0_BASE	UL(0x80000000)
#define	KUSEG_BASE	UL(0x00000000)

#define	PHYS_ADDR	0
#define	VIRT_ADDR	KSEG0_BASE

#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)

#ifndef	__ASSEMBLY__

#include <types/paddr_t.h>

static inline void *phys_to_virt(paddr_t phys)
{
	return (void *) phys + (VIRT_ADDR - PHYS_ADDR);
}

static inline paddr_t virt_to_phys(const void *virt)
{
	return (paddr_t) virt - (VIRT_ADDR - PHYS_ADDR);
}


#define	PFN_OFFSET	(PHYS_ADDR >> PAGE_SHIFT)

#define	__phys_to_pfn(phys)	((unsigned long) ((phys) >> PAGE_SHIFT))
#define	__pfn_to_phys(pfn)	(((paddr_t)(pfn)) << PAGE_SHIFT)

#endif

#endif
