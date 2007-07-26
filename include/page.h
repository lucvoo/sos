#ifndef	_PAGE_H_
#define	_PAGE_H_

#include <asm-ul.h>
#include <arch/page.h>

#define	PAGE_SIZE	(UL(1) << PAGE_SHIFT)
#define	PAGE_MASK	(~(PAGE_SIZE-1))
#define	PAGE_ALIGN(adr)	(((adr)+PAGE_SIZE-1)&PAGE_MASK)

#endif
