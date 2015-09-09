#ifndef	_SOC_CGU_H_
#define	_SOC_CGU_H_

#define	CGU_BASE	0x10000000
#define	CGU_SIZE	0x00000100

#define	CGU_CLKGR0	    0x0020

#define	CGU_CLKGR1	    0x0028
#define	CGU_CLKGR1_CORE1	(1 << 15)


#define	CGU_CPCCR	    0x0000

#define	CGU_LPCR	    0x0004
#define	CGU_LPCR_SCPU_PD	(1 << 31)
#define	CGU_LPCR_SCPU_ST	(1 << 27)

#define	CGU_CPPCR	    0x000C
#define	CGU_CPAPCR	    0x0010
#define	CGU_CPMPCR	    0x0014
#define	CGU_CPEPCR	    0x0018
#define	CGU_CPVPCR	    0x001C

#define	CGU_MSC0CDR	    0x0068

#define	CGU_MSC1CDR	    0x00A4
#define	CGU_MSC2CDR	    0x00A8

#endif
