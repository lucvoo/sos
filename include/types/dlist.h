#ifndef	_TYPES_DLIST_H_
#define	_TYPES_DLIST_H_


struct dlist {
	struct dlist *prev;
	struct dlist *next;
};

struct dlist_head {
	struct dlist	list;
};

#endif
