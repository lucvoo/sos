#ifndef	_ARCH_CPU_RELAX_H_
#define	_ARCH_CPU_RELAX_H_

#define	cpu_relax()	asm("yield" ::: "memory")

#endif
