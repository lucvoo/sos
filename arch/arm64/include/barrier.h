#ifndef	_ARCH_BARRIER_H_
#define	_ARCH_BARRIER_H_


#define	dmb(opt) asm volatile ("dmb " #opt : : : "memory")
#define	dsb(opt) asm volatile ("dsb " #opt : : : "memory")
#define	isb()    asm volatile ("isb" : : : "memory")

#include <generic/barrier.h>

#endif
