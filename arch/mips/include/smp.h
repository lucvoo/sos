#ifndef	_ARCH_SMP_H_
#define	_ARCH_SMP_H_

#include <soc/smp.h>
#include <arch/copro.h>


#define	__cpuid()	(c0_getval(c0_ebase) & 0x000003ff)

#endif
