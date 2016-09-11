#ifndef	_ARCH_SMP_H_
#define	_ARCH_SMP_H_

#include <soc/smp.h>
#include <arch/copro.h>


#define	__corenum()	(c0_getval(c0_ebase) & 0x000003ff)

#ifdef	CONFIG_MIPS_MT	// core == cpu

#define	NR_CPUS		NR_CORES
#define	__cpuid()	(__corenum() >> 1)
#define	__hwcpu(cpu)	((cpu) << 1)
#define	__vpeid(N)	0
#define	__coreid(N)	(N)

#else	// CONFIG_MIPS_MT

#define	__cpuid()	__corenum()

#endif	// CONFIG_MIPS_MT

#endif
