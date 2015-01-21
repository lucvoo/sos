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

#endif
