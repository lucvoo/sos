#ifndef	_ARCH_ASSEMBLY_H_
#define	_ARCH_ASSEMBLY_H_

#include <arch/regs-alias.h>


#define	DEFFUN(name, framesize, ra)	\
	.align	2;		\
	.type	name, @function;\
	.ent	name;		\
name:	.frame	sp, framesize, ra

#define	EXTFUN(name, framesize, ra)	\
	.globl	name;			\
	DEFFUN(name, framesize, ra)

#define	ENDFUN(name)		\
	.end	name;		\
	.size	name, .-name


#include <generic/assembly.h>

#endif
