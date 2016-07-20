#ifndef	_ARCH_BARRIER_H_
#define	_ARCH_BARRIER_H_


#define	dmb(opt) asm volatile ("dmb " #opt : : : "memory")
#define	dsb(opt) asm volatile ("dsb " #opt : : : "memory")
#define	isb()    asm volatile ("isb" : : : "memory")


#ifdef	CONFIG_SMP
#define	mb()		dmb(sy)
#define	mbr()		dmb(ld)
#define	mbw()		dmb(st)
#endif

#ifdef	CONFIG_SMP
#define	mb_smp()	dmb(ish)
#define	mbr_smp()	dmb(ishld)
#define	mbw_smp()	dmb(ishst)
#endif

#include <generic/barrier.h>

#endif
