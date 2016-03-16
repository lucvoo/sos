#ifndef	_GENERIC_BARRIER_H_
#define	_GENERIC_BARRIER_H_


#ifndef	barrier
#define barrier()	asm volatile("": : :"memory")
#endif


#ifndef	mb
#define	mb()	barrier()
#endif

#ifndef	mbr
#define	mbr()	mb()
#endif

#ifndef	mbw
#define	mbw()	mb()
#endif


#ifndef	CONFIG_SMP

#ifndef	mb_smp
#define	mb_smp()	barrier()
#endif

#ifndef	mbr_smp
#define	mbr_smp()	barrier()
#endif

#ifndef	mbw_smp
#define	mbw_sm()	barrier()
#endif

#else	// CONFIG_SMP

#ifndef	mb_smp
#define	mb_smp()	mb()
#endif

#ifndef	mbr_smp
#define	mbr_smp()	mbr()
#endif

#ifndef	mbw_smp
#define	mbw_smp()	mbw()
#endif

#endif	// CONFIG_SMP

#endif
