#ifndef	_SOC_SMP_H_
#define	_SOC_SMP_H_

#include <arch/copro.h>
#include <arch/regs-copro.h>

#define	__coreid()	(c0_getval(c0_ebase) & 0x00000007)

#endif
