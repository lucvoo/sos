#ifndef	_SMP_H_
#define	_SMP_H_


#ifndef	CONFIG_SMP
#define	__coreid()	0
#define	NR_CPUS		1
#else
#include <arch/smp.h>
#endif

#endif
