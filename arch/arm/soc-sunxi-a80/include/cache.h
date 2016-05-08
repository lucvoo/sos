#ifndef	_SOC_CACHE_H_
#define	_SOC_CACHE_H_

// WT WB RA WA
//  0  1  1  1

#define	CACHE_LI_SETS		128
#define	CACHE_LI_WAYS		4
#define	CACHE_LI_LINESIZE	64

#define	CACHE_LD_SETS		128
#define	CACHE_LD_WAYS		4
#define	CACHE_LD_LINESIZE	64


// LoUIS: L2
// LoC:	  L2
// LoUU:  L2

// Note: L2 must be used dynamically since
//	 it's not the same on the 2 clusters:
//	 512K / 2048 K

#endif
