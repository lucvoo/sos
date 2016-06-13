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

#endif
