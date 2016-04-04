#include <slab.h>
#include <page-alloc.h>
#include <page.h>
#include <lock.h>
#include <dlist.h>
#include <memory.h>
#include <align.h>
#include <atomic.h>
#include <init.h>


struct kmem_cache {
	struct lock	lock;
	void**		freelist;

	unsigned long	flags;
	unsigned int	cur_off;
	struct page*	cur_page;

	unsigned int	size;
	unsigned int	align;
	unsigned int	order;
	unsigned int	obj_off_max;

	const char*	name;

	struct dlist	list;
};



void *kmem_alloc(struct kmem_cache *c)
{
	struct page* page;
	unsigned long flags;
	void **p;

	flags = lock_acq_save(&c->lock);

	// 1) Try the freelist
	p = c->freelist;
	if (p) {
		c->freelist = *p;
		goto out;
	}

	// 2) Try the current allocating page
	if (c->cur_page && c->cur_off) {
do_page:
		c->cur_off -= c->size;
		p = page_to_virt(c->cur_page) + c->cur_off;
		goto out;
	}

	// 4) Need to get a new page
	page = page_alloc(c->order, GFP_KERN);
	if (page) {
		atomic_or(&page->flags, PG_slab);

		c->cur_page = page;
		c->cur_off = c->obj_off_max;
		goto do_page;
	}

out:
	lock_rel_rest(&c->lock, flags);
	return p;
}

void kmem_free(struct kmem_cache *c, const void *obj)
{
	void **p = (void *)(unsigned long)obj;
	unsigned long flags;

	flags = lock_acq_save(&c->lock);

	*p = c->freelist;
	c->freelist = p;

	// FIXME: must be able to return memory to the page allocator

	lock_rel_rest(&c->lock, flags);
}

/**********************************************************************/
/* kmem_cache creation & management                                   */

static struct kmem_cache kmem_meta_cache;
static DLIST_HEAD(kmem_caches);

static void kmem_cache_init(struct kmem_cache *c, unsigned int size,
			    unsigned int align)
{
	align = ALIGN(size, sizeof(void *));
	size = ALIGN(size, align);

	c->size = size;
	c->align = align;

	c->order = 0;
	if (size > PAGE_SIZE / 8)
		c->order = 2;

	c->obj_off_max = ((PAGE_SIZE << c->order) / size) * size;

	c->freelist = NULL;
	c->cur_page = NULL;
	c->cur_off = 0;

	lock_init(&c->lock);
	dlist_add_tail(&kmem_caches, &c->list);
}


struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
				     unsigned int align, unsigned long flags)
{
	struct kmem_cache *c;

	if (size > PAGE_SIZE)
		return NULL;
	if (align > PAGE_SIZE)
		return NULL;

	c = kmem_alloc(&kmem_meta_cache);
	if (!c)
		return c;

	c->name = name;
	c->flags = flags;
	kmem_cache_init(c, size, align);
	return c;
}


static void kmem_init(void)
{
	kmem_meta_cache.name = "kmem_cache";
	kmem_meta_cache.flags = 0;
	kmem_cache_init(&kmem_meta_cache, sizeof(kmem_meta_cache), 0);
}
pure_initcall(kmem_init);
