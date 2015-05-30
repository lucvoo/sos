#ifndef	_ARCH_NVIC_H_
#define	_ARCH_NVIC_H_

#include <arch/armv7m-scs.h>

#define	NVIC_BASE	SCS_BASE
#define	NVIC_SIZE	SCS_SIZE

#define	NVIC_BANK(I)	((I) / 32)

#define	NVIC_ISER(I)	(0x100 + NVIC_BANK(I) * 4)
#define	NVIC_ICER(I)	(0x180 + NVIC_BANK(I) * 4)
#define	NVIC_ISPR(I)	(0x200 + NVIC_BANK(I) * 4)
#define	NVIC_ICPR(I)	(0x280 + NVIC_BANK(I) * 4)
#define	NVIC_IABR(I)	(0x300 + NVIC_BANK(I) * 4)

#define	NVIC_IPR(I)	(0x400 + ((I) / 4) * 4)

#endif
