#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>


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

#include <generic/memory.h>

#endif

#endif
