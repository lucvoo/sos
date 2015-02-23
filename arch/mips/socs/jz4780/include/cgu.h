#ifndef	_SOC_CGU_H_
#define	_SOC_CGU_H_

#define	CGU_BASE	0x10000000
#define	CGU_SIZE	0x00000100

#define	CGU_CLKGR0	    0x0020

#define	CGU_CLKGR1	    0x0028
#define	CGU_CLKGR1_CORE1	(1 << 15)



#define	CGU_LPCR	    0x0004
#define	CGU_LPCR_SCPU_PD	(1 << 31)
#define	CGU_LPCR_SCPU_ST	(1 << 27)

#endif
