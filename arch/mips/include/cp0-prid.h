#ifndef	_ARCH_CP0_PRID_H_
#define	_ARCH_CP0_PRID_H_

#define	PRID_MASK(S)	(0xFF << (S))

#define	PRID_OPTS(X)	(((X) >> 24) & 0xFF)
#define	PRID_COMP(X)	(((X) >> 16) & 0xFF)
#define	PRID_PROC(X)	(((X) >>  8) & 0xFF)
#define	PRID_VERS(X)	(((X) >>  0) & 0xFF)


#define	PRID_COMP_LEGACY	0x00

#define	PRID_PROC_UNKNOWN		0xFF

#define	PRID_COMP_MIPS		0x01
#define	PRID_PROC_QEMU			0x00
#define	PRID_PROC_24K			0x93
#define	PRID_PROC_INTERAPTIV_UP		0xA0
#define	PRID_PROC_INTERAPTIV_MP		0xA1

#define	PRID_COMP_INGENIC_E1	0xE1	// jz4780
#define	PRID_PROC_JZRISC		0x02

#endif
