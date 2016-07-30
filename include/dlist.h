#ifndef	_DLIST_H_
#define	_DLIST_H_

#include <types/dlist.h>
#include <utils.h>
#include <stddef.h>


#define DLIST_HEAD_INIT(name)	{ .list = { &(name.list), &(name.list) } }
#define DLIST_HEAD(name)	struct dlist_head name = DLIST_HEAD_INIT(name)

#define dlist_entry(ptr, type, member)	container_of(ptr, type, member)
#define dlist_entry_null(ptr, type, member)	\
	({ struct dlist *p = ptr; p ? container_of(p, type, member) : NULL; })


static inline void __dlist_link(struct dlist *a, struct dlist *b)
{
	a->next = b;
	b->prev = a;
}


static inline void dlist_init_node(struct dlist *item)
{
	__dlist_link(item, item);
}

static inline void dlist_init(struct dlist_head *head)
{
	dlist_init_node(&head->list);
}


static inline void __dlist_add(struct dlist *prev, struct dlist *item, struct dlist *next)
{
	__dlist_link(prev, item);
	__dlist_link(item, next);
}

static inline void dlist_add_head(struct dlist_head *head, struct dlist *item)
{
	struct dlist *h = &head->list;

	__dlist_add(h, item, h->next);
}

static inline void dlist_add_tail(struct dlist_head *head, struct dlist *item)
{
	struct dlist *h = &head->list;

	__dlist_add(h->prev, item, h);
}


static inline void dlist_del(struct dlist *item)
{
	__dlist_link(item->prev, item->next);
}

static inline struct dlist* dlist_peek(struct dlist_head *head)
{
	struct dlist *h = &head->list;
	struct dlist *item = h->next;

	if (item == &head->list)
		return NULL;

	return item;
}
#define	dlist_peek_entry(head, type, member) dlist_entry_null(dlist_peek(head), type, member)

static inline struct dlist* dlist_pop(struct dlist_head *head)
{
	struct dlist *item = dlist_peek(head);

	if (item)
		dlist_del(item);
	return item;
}
#define	dlist_pop_entry(head, type, member) dlist_entry_null(dlist_pop(head), type, member)

static inline void dlist_replace(struct dlist *old, struct dlist *new)
{
	__dlist_link(old->prev, new);
	__dlist_link(new, old->next);
}


static inline void dlist_move_head(struct dlist_head *head, struct dlist *item)
{
        __dlist_link(item->prev, item->next);
        dlist_add_head(head, item);
}

static inline void dlist_move_tail(struct dlist_head *head, struct dlist *item)
{
        __dlist_link(item->prev, item->next);
        dlist_add_tail(head, item);
}


static inline void dlist_insert(struct dlist *prev, struct dlist *item, struct dlist *next)
{
	__dlist_add(prev, item, next);
}


static inline int dlist_is_first(const struct dlist_head *head, const struct dlist *item)
{
	return item->prev == &head->list;
}

static inline int dlist_is_last(const struct dlist_head *head, const struct dlist *item)
{
	return item->next == &head->list;
}

static inline int dlist_is_empty(const struct dlist_head *head)
{
	const struct dlist *h = &head->list;

	return h->next == h;
}


#define dlist_foreach(pos, head) \
	for (pos = (head)->list.next; pos != &(head)->list; pos = pos->next)

#define dlist_foreach_safe(pos, next, head) \
	for (pos = (head)->list.next; next = pos->next, pos != &(head)->list; pos = next)


#define dlist_foreach_entry(pos, head, member)				\
	for (pos = dlist_entry((head)->list.next, typeof(*pos), member);	\
	     &pos->member != &(head)->list; 					\
	     pos = dlist_entry(pos->member.next, typeof(*pos), member))


#define dlist_foreach_entry_safe(pos, next, head, member)			\
	for (pos = dlist_entry((head)->list.next, typeof(*pos), member);	\
	     next = dlist_entry(pos->member.next, typeof(*pos), member),	\
	     &pos->member != &(head)->list;					\
	     pos = next)

#endif
