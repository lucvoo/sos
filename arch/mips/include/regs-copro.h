#ifndef	_MIPS_REGS_H_
#define	_MIPS_REGS_H_


#define	c0_context	$4


#define	c0_status	$12
#define		ST0_IE		(1 << 0)
#define		ST0_EXL		(1 << 1)
#define		ST0_ERL		(1 << 2)
#define		ST0_KSU		(3 << 3)
#define		ST0_CU0		(1 <<28)

#endif
