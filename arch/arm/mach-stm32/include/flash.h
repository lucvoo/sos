#ifndef	_MACH_FLASH_H_
#define	_MACH_FLASH_H_

#define	FLASH_ACR		0x00
#define	FLASH_ACR_LATENCY(X)	((X) << 0)
#define	FLASH_ACR_PRFTEN	(1 <<  8)
#define	FLASH_ACR_ICEN		(1 <<  9)
#define	FLASH_ACR_DCEN		(1 << 10)

#endif
