#include <page-alloc.h>
#include <page.h>
#include <memory.h>
#include <dlist.h>
#include <lock.h>
#include <utils/array-iter.h>
#include <init.h>


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
