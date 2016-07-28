#ifndef	_ARCH_ASSEMBLY_H_
#define	_ARCH_ASSEMBLY_H_

#ifdef	__ASSEMBLY__
	.syntax	unified

#define	ASM_TYPE(type)	\
	%type

#include <generic/assembly.h>

#ifdef	CONFIG_ARMV6K
#include <arch/cp15.h>

	.macro	isb
#ifdef	CONFIG_SMP
		mcr	CP15ISB(r0)
#else
		nop
#endif
	.endm
#endif

#else	// __ASSEMBLY__

#ifdef	CONFIG_THUMB2
#define	LLSC_MODE	"Qo"
#else
#define	LLSC_MODE	"Q"
#endif

#endif	// __ASSEMBLY__

#endif
