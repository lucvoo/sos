#ifndef	_ASM_UL_H_
#define	_ASM_UL_H_

#ifndef	__ASSEMBLY__
#define	UL(x)	__UL(x)
#define	__UL(x)	(x##UL)
#else
#define	UL(x)	(x)
#endif

#endif
