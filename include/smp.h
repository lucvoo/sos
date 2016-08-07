#ifndef	_SMP_H_
#define	_SMP_H_


#ifndef	CONFIG_SMP
#define	__cpuid()	0
#define	NR_CPUS		1
#else
#include <arch/smp.h>

void smp_ipi_schedule_one(unsigned int cpu);
void smp_ipi_schedule_all(void);
#endif

#define	foreach_cpu(cpu)	for (uint cpu = 0; cpu < NR_CPUS; ++cpu)

#endif
