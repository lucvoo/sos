#ifndef	_ARCH_ASSEMBLY_H_
#define	_ARCH_ASSEMBLY_H_

#ifdef	__ASSEMBLY__
	.syntax	unified

#define	ASM_TYPE(type)	\
	%type

#include <generic/assembly.h>

#else	// __ASSEMBLY__

#ifdef	CONFIG_THUMB2
#define	LLSC_MODE	"Qo"
#else
#define	LLSC_MODE	"Q"
#endif

#endif	// __ASSEMBLY__

#endif
