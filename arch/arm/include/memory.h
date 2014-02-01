#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#include <asm-ul.h>
#include "arch/mach/memory.h"

#ifndef	VIRT_ADDR
#define	VIRT_ADDR	UL(CONFIG_VIRT_ADDR)		// Virtual RAM. Same as Linux's PAGE_OFFSET
#endif
#ifndef	TEXT_OFFSET
#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)
#endif

#endif
