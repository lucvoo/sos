#ifndef	_ARCH_BARRIER_H_
#define	_ARCH_BARRIER_H_


#if CONFIG_ARM_ARCH >= 7
#define	dmb(opt) asm volatile ("dmb " #opt : : : "memory")
#define	dsb(opt) asm volatile ("dsb " #opt : : : "memory")
#define	isb(opt) asm volatile ("isb " #opt : : : "memory")
#elif CONFIG_ARM_ARCH == 6
#include <arch/cp15.h>
#define	dmb(opt) asm volatile ("mcr " STRINGIFY(CP15DMB(%0)): : "r" (0) : "memory")
#define	dsb(opt) asm volatile ("mcr " STRINGIFY(CP15DSB(%0)): : "r" (0) : "memory")
#define	isb(opt) asm volatile ("mcr " STRINGIFY(CP15ISB(%0)): : "r" (0) : "memory")
#endif


#ifdef	CONFIG_SMP
#define	mb()		dmb();
#define	mbr()		dmb();
#define	mbw()		dmb(st);
#endif

#if defined(CONFIG_DMA_NEED_BARRIER)
#define	mbr_dma()	dmb(osh);
#define	mbw_dma()	dmb(oshst);
#endif

#ifdef	CONFIG_SMP
#define	mb_smp()	dmb(ish);
#define	mbr_smp()	dmb(ish);
#define	mbw_smp()	dmb(ishst);
#endif

#include <generic/barrier.h>

#endif
