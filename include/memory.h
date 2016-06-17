#ifndef	_MEMORY_H_
#define	_MEMORY_H_

#include <page.h>
#include "arch/memory.h"

// Simple flat, linear, continuous memory layout
extern struct page pagemap[];

#define	pfn_to_page(pfn)	&pagemap[pfn]
#define	page_to_pfn(pag)	((pag) - pagemap)

#define	page_to_phys(pag)	__pfn_to_phys(page_to_pfn(pag))
#define	page_to_virt(pag)	phys_to_virt(page_to_phys(pag))
#define	phys_to_page(pad)	pfn_to_page(__phys_to_pfn(pad))
#define	virt_to_page(ptr)	phys_to_page(virt_to_phys(ptr))

#endif
