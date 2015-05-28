#ifndef	_MACH_PWR_H_
#define	_MACH_PWR_H_

#define	PWR_BASE	0x40007000
#define	PWR_SIZE	     0x400


#define	PWR_CR		0x00
#define	PWR_CR_VOS		(1 << 14)

#define	PWR_CSR		0x04
#define	PWR_CSR_VOS		(1 << 14)

#endif
