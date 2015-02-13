#ifndef	_ARCH_BARRIER_H_
#define	_ARCH_BARRIER_H_


#ifdef CONFIG_CPU_HAS_SYNC
#define	__arch_mb() asm volatile ("sync" : : : "memory")
#else
#error FIXME
#endif


#ifdef	CONFIG_SMP
#define	mb()		__arch_mb();
#define	mbr()		__arch_mb();
#define	mbw()		__arch_mb();
#endif

#ifdef	CONFIG_SMP
#define	mb_smp()	barrier();
#define	mbr_smp()	barrier();
#define	mbw_smp()	barrier();
#endif

#include <generic/barrier.h>

#endif
