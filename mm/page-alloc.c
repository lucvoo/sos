#include <page-alloc.h>
#include <page.h>
#include <memory.h>
#include <dlist.h>
#include <lock.h>
#include <utils/array-iter.h>
#include <atomic.h>
#include <init.h>
#include <debug.h>


#define	MAX_ORDER	12

struct page_freelist {
	struct dlist_head	head;
	struct lock		lock;
};

static struct page_freelist page_freelists[MAX_ORDER+1];


static void __init page_alloc_init(void)
{
	struct page_freelist *pfl;

	for_each_array_item(pfl, page_freelists) {
		dlist_init(&pfl->head);
		lock_init(&pfl->lock);
	}
}
pure_initcall(page_alloc_init);


/**********************************************************************************************/
// Check consistency of the pages in the freelists

static int page_alloc_verify_page(struct page *p, unsigned int order, int i, unsigned long flags)
{
	if (p[i].flags != flags) {
		dbg("%s(%p:%lx, %u) %p:%lx->flags is %lx instead of %u\n", __func__,
			p, page_to_pfn(p), order, &p[i], page_to_pfn(&p[i]), p[i].flags, flags);
		return 1;
	}

	return 0;
}

static int page_alloc_verify_block(struct page *p, unsigned int order)
{
	unsigned long size = 1 << order;
	unsigned int i;
	int rc;

	rc = page_alloc_verify_page(p, order, 0, PG_free);

	for (i = 1; i < size; i++ )
		rc += page_alloc_verify_page(p, order, i, 0x0);

	return rc;
}

static int __used page_alloc_verify(void)
{
	unsigned int order;
	int rc = 0;

	for (order = 0; order <= MAX_ORDER; order++) {
		struct page_freelist *fl = &page_freelists[order];
		struct page *p;

		lock_acq(&fl->lock);
		dlist_foreach_entry(p, &fl->head, list) {
			rc += page_alloc_verify_block(p, order);
		}
		lock_rel(&fl->lock);
	}

	return rc;
}
/**********************************************************************************************/


static void freelist_rem(struct page *p, unsigned int order)
{
	struct page_freelist *fl = &page_freelists[order];

	lock_acq(&fl->lock);
	dlist_del(&p->list);
	lock_rel(&fl->lock);
}

static void freelist_add(struct page *p, unsigned int order)
{
	struct page_freelist *fl = &page_freelists[order];

	lock_acq(&fl->lock);
	dlist_add_head(&fl->head, &p->list);
	lock_rel(&fl->lock);
}


static unsigned long pfn_buddy(unsigned long pfn, unsigned int order)
{
	return pfn ^ (1 << order);
}

static unsigned long pfn_parent(unsigned long pfn, unsigned int order)
{
	return pfn & ~(1 << order);
}


void page_free(struct page *p, unsigned int order)
{
	unsigned long pfn = page_to_pfn(p);

	while (order < MAX_ORDER) {
		struct page *buddy = pfn_to_page(pfn_buddy(pfn, order));

		// test if page is in the buddy system
		if (!atomic_clr(&buddy->flags, PG_free))
			break;

		// if order doesn't match, mark it back as free
		if (buddy->flags != order) {
			atomic_or(&buddy->flags, PG_free);
			break;
		}

		// buddy is free, we can merge
		freelist_rem(buddy, order);

		// now play with the parent
		pfn = pfn_parent(pfn, order++);
	}

	p = pfn_to_page(pfn);
	freelist_add(p, order);
	p->order = order;
	atomic_xor(&p->flags, PG_free);
}


static struct page *freelist_pop(unsigned int order)
{
	struct page_freelist *fl = &page_freelists[order];
	struct page *p;

	lock_acq(&fl->lock);
	dlist_foreach_entry(p, &fl->head, list) {
		if (!atomic_clr(&p->flags, PG_free))
			continue;
		dlist_del(&p->list);
		goto found;
	}
	p = NULL;

found:
	lock_rel(&fl->lock);

	return p;
}

struct page *page_alloc(unsigned int order, unsigned int aflags)
{
	struct page *p;

	if (order > MAX_ORDER)
		return NULL;

	p = freelist_pop(order);
	if (p)
		return p;

	// try an higher order block and split it
	p = page_alloc(order+1, aflags);
	if (p)
		page_free(p + (1 << order), order);

	return p;
}
