#ifndef	_ARCH_SMP_H_
#define	_ARCH_SMP_H_

#define	MPIDR_CORE(R)		(((R) >> 0) & 0xff)
#define	MPIDR_CLUSTER(R)	(((R) >> 8) & 0xff)

#include <arch/msr.h>
#include <soc/smp.h>


static inline uint __coreid(void)
{
	u32 mpidr = msr_getval(mpidr_el1);
	uint cluster = MPIDR_CLUSTER(mpidr);
	uint core = MPIDR_CORE(mpidr);

	return (cluster * 4) | core;
}

#endif
