#include <page-alloc.h>
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
