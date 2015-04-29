#include <net/skb.h>
#include <page-alloc.h>
#include <page.h>
#include <memory.h>
#include <round.h>


// FIXME: this should be changed to:
//	1) allocate the skb from a specific kmem_cache
//	2) we can allocate the buffer from a (big) block,
//	   like from a stack and just increment an usage count.
//	   To free it, just dec the usage count and free the block
//	   when the usage count is zero.
struct skb *skb_alloc(unsigned int len, unsigned int reserve)
{
	unsigned int skblen = ALIGN(sizeof(struct skb), 4);
	unsigned int totlen = skblen + len + reserve;
	struct skb *skb;
	struct page *p;
	void *buff;

	if (totlen > PAGE_SIZE)
		return NULL;

	p = page_alloc(0, GFP_KERN);
	if (!p) {
		return NULL;
	}
	buff = page_to_virt(p);

	skb  = buff;
	buff += skblen;
	skb->buff = buff;
	skb->size = len + reserve;
	skb->data = buff + reserve;
	skb->len = 0;

	return skb;
}

void skb_free(struct skb *skb)
{
	struct page *p = virt_to_page(skb);

	page_free(p, 0);
}


struct skb *skb_alloc_tx(struct netdev *dev, unsigned int len)
{
	unsigned int hlen = 16;		// FIXME
	struct skb *skb;

	skb = skb_alloc(len, hlen);
	return skb;
}
