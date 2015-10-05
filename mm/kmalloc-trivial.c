#include "kmalloc-internal.h"
#include <kmalloc.h>
#include <page-alloc.h>
#include <atomic.h>
#include <round.h>
#include <page.h>
#include <memory.h>
#include <lock.h>


/*
 * Kinda stupid, at least ultra-simple memory allocator.
 * It's backed, of course, by the page allocator.
 * Inside a page (possibly a compound one), no memory accounting
 * is done but the total size allocated.
 * In consequence, memory inside the page can't be reused: when a new page
 * is allocated: 2 counters inside the page structure are initialized:
 *	* free space, initialized to the size of the page
 *	* used space, initialized to 0.
 * At allocation time, the free space is decremented and the used space is
 * incremented by the size to be allocated.
 * If not enough free space remains a new page is allocated.
 * At freeing time the used space is decremented and the page is freed when this
 * counter reach zero.
 * This may not be cache friendly but is very simple.
 *
 * NOTE: an internal reference count is also taken for the page we're currently
 *       allocating.
 * FIXME: take the gfp flags in account (need 1 current page per kind)
 */

#define	KMALLOC_PAGE_ORDER	0
#define	KMALLOC_PAGE_SIZE	(PAGE_SIZE << KMALLOC_PAGE_ORDER)

struct kmalloc_ent {
	u32		size;
	u8		data[];
};

static struct kmalloc_state {
	struct page	*curr_page;
	struct lock	lock;
} kmalloc_state = {
	.curr_page = NULL,
	.lock = LOCK_INIT,
};


static void kmalloc_put_page(struct page *page, uint cnt)
{
	if (atomic_sub_return(&page->count, cnt))
		return;

	// free the page
	pr_info("rel empty page: %p -> %p\n", page, page_to_virt(page));
	atomic_xor(&page->flags, PG_kmalloc);
	page_free(page, KMALLOC_PAGE_ORDER);
}

void *__kmalloc(uint size, uint aflags)
{
	struct kmalloc_state *state = &kmalloc_state;
	struct kmalloc_ent *ent;
	struct page *page;
	void *ptr = NULL;
	ulong flags;

	if (size > KMALLOC_PAGE_SIZE/2)
		return NULL;

	size += sizeof(ent->size);

	flags = lock_acq_save(&state->lock);
	page = state->curr_page;
	if (!page || (page->order < size)) {

		if (page)
			kmalloc_put_page(page, 1);

		page = page_alloc(KMALLOC_PAGE_ORDER, aflags);
		if (!page) {
			pr_err("page_alloc() failed!\n");
			goto end;
		}

		pr_info("got a new page: %p -> %p\n", page, page_to_virt(page));
		state->curr_page = page;
		atomic_or(&page->flags, PG_kmalloc);
		page->order = KMALLOC_PAGE_SIZE;
		atomic_set(&page->count, 1);
	}

	page->order -= size;
	atomic_add(&page->count, size);
	ent = page_to_virt(page) + page->order;
	ent->size = size;
	ptr = &ent->data;
	pr_dbg("-> %p, %03x (count = %03x, order = %04x)\n", ptr, size, page->count, page->order);

end:
	lock_rel_rest(&state->lock, flags);
	pr_dbg("-> %p\n", ptr);
	return ptr;
}

void kfree(const void *ptr)
{
	const struct kmalloc_ent *ent = container_of(ptr, typeof(*ent), data[0]);
	struct page *page = virt_to_page(ent);
	uint size = ent->size;

	if (!ptr)
		return;

	// FIXME: validate ptr
	// FIXME: validate ent->size
	// FIXME: validate page & page->flags

	pr_dbg("<- %p, %03x (count = %03x, order = %04x)\n", ptr, size, page->count, page->order);
	kmalloc_put_page(page, size);
}
