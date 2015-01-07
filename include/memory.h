#ifndef	_MEMORY_H_
#define	_MEMORY_H_

#include <page.h>
#include "arch/memory.h"

// Simple flat, linear, continuous memory layout
extern struct page pagemap[];

#define	pfn_to_page(pfn)	&pagemap[(pfn) - PFN_OFFSET]

#endif
