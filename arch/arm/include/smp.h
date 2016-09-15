#ifndef	_ARCH_SMP_H_
#define	_ARCH_SMP_H_

#include <arch/cp15.h>
#include <soc/smp.h>


static inline uint __cpuid(void)
{
	u32 mpidr = cp_read(MPIDR);
	uint cluster = MPIDR_CLUSTER(mpidr);
	uint core = MPIDR_CORE(mpidr);

	return (cluster * 4) | core;
}

#endif
