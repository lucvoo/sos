#ifndef _ARCH_EL2_H_
#define _ARCH_EL2_H_

#include <bits.h>


#define	HCR_MIOCNCE	(1 << 38)
#define	HCR_ID		(1 << 33)
#define	HCR_CD		(1 << 32)
#define	HCR_RW		(1 << 31)
#define	HCR_TRVM	(1 << 30)
#define	HCR_HCD		(1 << 29)
#define	HCR_TDZ		(1 << 28)
#define	HCR_TGE		(1 << 27)
#define	HCR_TVM		(1 << 26)
#define	HCR_TTLB	(1 << 25)
#define	HCR_TPU		(1 << 24)
#define	HCR_TPC		(1 << 23)
#define	HCR_TSW		(1 << 22)
#define	HCR_TACR	(1 << 21)
#define	HCR_TIDCP	(1 << 20)
#define	HCR_TSC		(1 << 19)
#define	HCR_TID3	(1 << 18)
#define	HCR_TID2	(1 << 17)
#define	HCR_TID1	(1 << 16)
#define	HCR_TID0	(1 << 15)
#define	HCR_TWE		(1 << 14)
#define	HCR_TWI		(1 << 13)
#define	HCR_DC		(1 << 12)
#define	HCR_BSU		BITS(10, 2)
#define	HCR_FB		(1 << 9)
#define	HCR_VSE		(1 << 8)
#define	HCR_VI		(1 << 7)
#define	HCR_VF		(1 << 6)
#define	HCR_AMO		(1 << 5)
#define	HCR_IMO		(1 << 4)
#define	HCR_FMO		(1 << 3)
#define	HCR_PTW		(1 << 2)
#define	HCR_SWIO	(1 << 1)
#define	HCR_VM		(1 << 0)

#endif
