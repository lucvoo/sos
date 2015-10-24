#ifndef	_TYPES_WAITER_H_
#define	_TYPES_WAITER_H_

#include <types/dlist.h>


struct thread;

struct waiter {
	struct dlist	node;
	struct thread	*thread;
};

#endif
