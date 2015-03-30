#ifndef	_CIRCBUF_H_
#define	_CIRCBUF_H_


struct circbuf {
	unsigned int	head;	// consumer end
	unsigned int	tail;	// producer
};


static inline unsigned int circbuf_empty(struct circbuf *buf)
{
	return buf->head == buf->tail;
}

static inline unsigned int circbuf_busy(struct circbuf *buf, unsigned int order)
{
	return (buf->tail - buf->head) % (1 << order);
}

static inline unsigned int circbuf_free(struct circbuf *buf, unsigned int order)
{
	return (1 << order) - circbuf_busy(buf, order) - 1;
}

static inline unsigned int circbuf_inc_head(struct circbuf *buf, unsigned int order)
{
	return buf->head = (buf->head + 1) % (1 << order);
}

static inline unsigned int circbuf_inc_tail(struct circbuf *buf, unsigned int order)
{
	return buf->tail = (buf->tail + 1) % (1 << order);
}

#define	circbuf_iter_head(H, BUF, ORDER)	\
	for (H = (BUF)->head; H != (BUF)->tail; H = (BUF)->head = ((H) + 1) % (1 << ORDER))

#endif
