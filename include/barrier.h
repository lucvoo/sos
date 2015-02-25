#ifndef	_BARRIER_H_
#define	_BARRIER_H_

#include <arch/barrier.h>

#define	mb_get(X)		*((volatile typeof(X) *) &(X))

#endif
