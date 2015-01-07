#ifndef	_PAGE_ALLOC_H_
#define	_PAGE_ALLOC_H_


struct page;

void page_free(struct page *p, unsigned int order);

#endif
