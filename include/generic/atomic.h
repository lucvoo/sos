#ifndef	_GENERIC_ATOMIC_H_
#define	_GENERIC_ATOMIC_H_


#include <types/atomic.h>


#define	__atomic_val(p)		(p)->val

#include <generic/atomic-gcc.h>

#endif
