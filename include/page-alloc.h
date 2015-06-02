#ifndef	_PAGE_ALLOC_H_
#define	_PAGE_ALLOC_H_

#include <alloc-flags.h>


struct page;

struct page *page_alloc(unsigned int order, unsigned int aflags);
void page_free(struct page *p, unsigned int order);

#endif
