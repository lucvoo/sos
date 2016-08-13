#ifndef	_SOC_SMP_H_
#define	_SOC_SMP_H_

#include <arch/copro.h>
#include <arch/regs-copro.h>

#define	__cpuid()	(c0_getval(c0_ebase) & 0x00000007)

#define	NR_CPUS		2

#define	SMP_ENTRY_ALIGN	0x10000

#endif
