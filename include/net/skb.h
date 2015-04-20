#ifndef	_NET_SKB_H_
#define	_NET_SKB_H_

#include <dlist.h>
#include <lock.h>


struct skb_queue {
	struct dlist_head	head;

	unsigned int		len;
	struct lock		lock;
};


struct skb {
	struct dlist		node;
	struct netdev		*dev;

	unsigned short		proto;

	void *			buff;
	void *			data;
	unsigned int		len;
	unsigned int		size;
};

/*
 * skb aliases:
 * .tail := .data + .len
 * .end  := .buff + .size
 *
 * skb invariants:
 * .data >= .buff
 * .tail <= .end (.data + .len <= .buff + .size)
 */

/******************************************************************************/

struct skb *skb_alloc(unsigned int len, unsigned int reserve);
void skb_free(struct skb *skb);

/******************************************************************************/

/**
 * @returns a pointer after the valid data
 */
static inline unsigned char *skb_tail(const struct skb *skb)
{
	return skb->data + skb->len;
}

/**
 * @returns a pointer after the valid buffer
 */
static inline unsigned char *skb_end(const struct skb *skb)
{
	return skb->buff + skb->size;
}


/**
 * Increase the headroom of an empty &skb.
 *
 * Obviously, this must only be done before some data are added.
 */
static inline void skb_reserve(struct skb *skb, int len)
{
	skb->data += len;
}

/**
 * Prepare to add data to a skb's head
 * @returns a pointer to where to write the data
 */
static inline void *skb_add_head(struct skb *skb, unsigned int len)
{
	skb->len  += len;
	return skb->data -= len;
}

/**
 * Prepare to add data to a skb's tail
 * @returns a pointer to where to write the data
 */
static inline void *skb_add_tail(struct skb *skb, unsigned int len)
{
	void *data = skb_tail(skb);

	skb->len  += len;
	return data;
}

/**
 * Add a skb at the tail of the queue
 * @skq: the queue
 * @skb: the buffer
 */
static inline void skb_enqueue_tail(struct skb_queue *q, struct skb *skb)
{
	dlist_add_tail(&q->head, &skb->node);
}

/**
 * Remove a skb from a queue
 */
static struct skb *skb_dequeue_head(struct skb_queue *q)
{
	return dlist_pop_entry(&q->head, struct skb, node);
}

#endif
