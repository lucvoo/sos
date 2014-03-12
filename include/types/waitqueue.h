#ifndef	_TYPES_WAITQUEUE_H_
#define	_TYPES_WAITQUEUE_H_

#include <types/dlist.h>
#include <lock.h>


struct waitqueue {
	struct dlist_head	waiters;
	struct lock	lock;
};

#endif
