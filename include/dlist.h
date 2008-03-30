#ifndef	_DLIST_H_
#define	_DLIST_H_

#include <types/dlist.h>
#include <utils.h>


#define DLIST_INIT(name)	{ &(name), &(name) }
#define DLIST(name)		struct dlist name = DLIST_INIT(name)

#define dlist_entry(ptr, type, member)	container_of(ptr, type, member)


static inline void __dlist_link(struct dlist *a, struct dlist *b)
{
	a->next = b;
	b->prev = a;
}

static inline void dlist_init(struct dlist *item)
{
	__dlist_link(item, item);
}


static inline void __dlist_add(struct dlist *prev, struct dlist *item, struct dlist *next)
{
	__dlist_link(prev, item);
	__dlist_link(item, next);
}

static inline void dlist_add_head(struct dlist *head, struct dlist *item)
{
	__dlist_add(head, item, head->next);
}

static inline void dlist_add_tail(struct dlist *head, struct dlist *item)
{
	__dlist_add(head->prev, item, head);
}


static inline void dlist_del(struct dlist *item)
{
	__dlist_link(item->prev, item->next);
}

static inline struct dlist* dlist_pop(struct dlist* head)
{
	struct dlist* item = head->next;

	dlist_del(item);
	return item;
}
#define	dlist_pop_entry(head, type, member) dlist_entry(dlist_pop(head), type, member)

static inline void dlist_replace(struct dlist *old, struct dlist *new)
{
	__dlist_link(old->prev, new);
	__dlist_link(new, old->next);
}


static inline void dlist_move_head(struct dlist *head, struct dlist *item)
{
        __dlist_link(item->prev, item->next);
        dlist_add_head(head, item);
}

static inline void dlist_move_tail(struct dlist *head, struct dlist *item)
{
        __dlist_link(item->prev, item->next);
        dlist_add_tail(head, item);
}


static inline int dlist_is_first(const struct dlist *head, const struct dlist *item)
{
	return item->prev == head;
}

static inline int dlist_is_last(const struct dlist *head, const struct dlist *item)
{
	return item->next == head;
}

static inline int dlist_is_empty(const struct dlist *head)
{
	return head->next == head;
}


#define dlist_foreach(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)


#define dlist_foreach_entry(pos, head, member)				\
	for (pos = dlist_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = dlist_entry(pos->member.next, typeof(*pos), member))

#endif
