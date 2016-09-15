#ifndef	_ARCH_BARRIER_H_
#define	_ARCH_BARRIER_H_


#ifdef CONFIG_CPU_HAS_SYNC
#define	__mips_sync(type)	asm volatile ("sync " #type : : : "memory")
#define	__arch_sync()		__mips_sync(0x00)
#define	__arch_mb()		__mips_sync(0x10)
#define	__arch_mbw()		__mips_sync(0x04)
#define	__arch_mbr()		__mips_sync(0x13)
#define	__arch_mb_acq()		__mips_sync(0x11)
#define	__arch_mb_rel()		__mips_sync(0x12)
#else
#error FIXME
#endif


#define	mb()		__arch_sync();
#define	mbr()		__arch_sync();
#define	mbw()		__arch_sync();

#ifdef	CONFIG_SMP
#define	mb_smp()	__arch_mb();
#define	mbr_smp()	__arch_mbr();
#define	mbw_smp()	__arch_mbw();
#endif

#include <generic/barrier.h>

#endif
