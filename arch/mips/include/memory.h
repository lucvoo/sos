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

#include <generic/memory.h>

static inline void *phys_to_kseg1(paddr_t phys)
{
	return (void *) phys + (KSEG1_BASE - PHYS_ADDR);
}

static inline ulong virt_to_kseg1(const void *virt)
{
	return ((ulong) virt) + (KSEG1_BASE - VIRT_ADDR);
}

#endif

#endif
