#include <net/skb.h>
#include <page-alloc.h>
#include <page.h>
#include <slab.h>
#include <memory.h>
#include <round.h>
#include <init.h>


// FIXME: Instead of allocating a full page for each packet
//	we should allocate the buffer from a (big) block,
//	like from a stack and just increment an usage count.
//	To free it, just dec the usage count and free the block
//	when the usage count is zero.

static struct kmem_cache *skb_cache;

static void skb_cache_init(void)
{
	skb_cache = kmem_cache_create("skb", sizeof(struct skb), 0, 0);
	if (!skb_cache) {
		pr_err("failed to create skb_cache");
	}
}
core_initcall(skb_cache_init);


struct skb *skb_alloc(unsigned int len, unsigned int reserve)
{
	unsigned int totlen = len + reserve;
	struct skb *skb;
	struct page *p;
	void *buff;

	if (totlen > PAGE_SIZE)
		return NULL;

	skb = kmem_alloc(skb_cache);
	if (!skb)
		return NULL;

	p = page_alloc(0, GFP_KERN);
	if (!p) {
		kmem_free(skb_cache, skb);
		return NULL;
	}
	buff = page_to_virt(p);

	skb->buff = buff;
	skb->size = len + reserve;
	skb->data = buff + reserve;
	skb->len = 0;

	return skb;
}

void skb_free(struct skb *skb)
{
	struct page *p = virt_to_page(skb->buff);

	page_free(p, 0);
	kmem_free(skb_cache, skb);
}


struct skb *skb_alloc_tx(struct netdev *dev, unsigned int len)
{
	unsigned int hlen = 16;		// FIXME
	struct skb *skb;

	skb = skb_alloc(len, hlen);
	return skb;
}
