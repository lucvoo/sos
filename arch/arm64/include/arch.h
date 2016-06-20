#ifndef _ARCH_ARCH_H_
#define _ARCH_ARCH_H_

#include <bits.h>


#define DAIF_D		0x8
#define DAIF_A		0x4
#define DAIF_I		0x2
#define DAIF_F		0x1

#define PSR_NZCV	BITS(28, 4)
#define PSR_SS		(1 << 21)
#define PSR_IL		(1 << 20)
#define PSR_E		(1 << 9)
#define PSR_D		(1 << 9)
#define PSR_A		(1 << 8)
#define PSR_I		(1 << 7)
#define PSR_F		(1 << 6)
#define PSR_DAIF	(0xf << 6)
#define PSR_AA32	(1 << 4)
#define PSR_EL		BITS(2, 2)
#define PSR_EL3		(3 << 2)
#define PSR_EL2		(2 << 2)
#define PSR_EL1		(1 << 2)
#define PSR_EL0		(0 << 2)
#define PSR_SPh		(1 << 0)

#define	SCTLR_UCI	(1 << 26)
#define	SCTLR_EE	(1 << 25)
#define	SCTLR_E0E	(1 << 24)
#define	SCTLR_WXN	(1 << 19)
#define	SCTLR_nTWE	(1 << 18)
#define	SCTLR_nTWI	(1 << 16)
#define	SCTLR_UCT	(1 << 15)
#define	SCTLR_DZE	(1 << 14)
#define	SCTLR_I		(1 << 12)
#define	SCTLR_UMA	(1 << 9)
#define	SCTLR_SED	(1 << 8)
#define	SCTLR_ITD	(1 << 7)
#define	SCTLR_CP15EN	(1 << 5)
#define	SCTLR_SA0	(1 << 4)
#define	SCTLR_SA	(1 << 3)
#define	SCTLR_C		(1 << 2)
#define	SCTLR_A		(1 << 1)
#define	SCTLR_M		(1 << 0)


#define	CPTR_TCPAC	(1 << 31)
#define	CPTR_TTA	(1 << 20)
#define	CPTR_TFP	(1 << 10)


#define	CLIDR_ICB	BITS(30, 3)
#define	CLIDR_LoUU	BITS(27, 3)
#define	CLIDR_LoC	BITS(24, 3)
#define	CLIDR_LoUIS	BITS(21, 3)
#define	CLIDR_CTYPE(N)	BITS(((N)-1)*3, 3)
#define	CLIDR_CTYPE_NONE	0b000
#define	CLIDR_CTYPE_ICACHE	0b001
#define	CLIDR_CTYPE_DCACHE	0b010
#define	CLIDR_CTYPE_BOTH	0b011
#define	CLIDR_CTYPE_UNIFIED	0b100

#define	CSSELR_LEVEL	BITS(1, 3)

#define	CCSIDR_LINESIZE	BITS(0, 3)
#define	CCSIDR_NWAYS	BITS(3, 10)
#define	CCSIDR_NSETS	BITS(13, 15)

#define	DCISW_LEVEL	BITS(1, 3)
#define	DCISW_TOPBIT	32

#endif
