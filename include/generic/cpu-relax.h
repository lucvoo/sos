#ifndef	_GENERIC_CPU_RELAX_H_
#define	_GENERIC_CPU_RELAX_H_

#ifndef	cpu_relax
#define	cpu_relax()	asm("" ::: "memory")
#endif

#endif
