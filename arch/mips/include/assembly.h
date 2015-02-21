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


.macro	GLOB	name, glob
	.ifc	\glob,global
	.global	\name
	.endif
.endm


#define	ENTRY(name, glob)	\
	GLOB	name, glob;	\
	.type	name, @function;\
	.ent	name;		\
name:


#include <generic/assembly.h>

#endif
