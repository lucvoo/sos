#ifndef	_ARCH_CACHEOP_H_
#define	_ARCH_CACHEOP_H_

// define for the 'cache' instruction
#define	CACHEOP_LI	0x0	// L1 I-cache
#define	CACHEOP_LD	0x1	// L1 D-cache (or unified cache)
#define	CACHEOP_L2	0x2	// L2 cache
#define	CACHEOP_L3	0x3	// L3 cache


#define	CACHEOP_IDX_INV	(0<<2)	// Invalidate		   by Index
#define	CACHEOP_IDX_WBI	(0<<2)	// Write-Back & Invalidate by Index
#define	CACHEOP_IDX_LTA	(1<<2)	// Load Tag		   by Index
#define	CACHEOP_IDX_STA	(2<<2)	// Store Tag		   by Index
#define	CACHEOP_HIT_INV	(4<<2)	// Invalidate		   by address
#define	CACHEOP_FILL	(5<<2)	// Fill			   by address
#define	CACHEOP_HIT_WBI	(5<<2)	// Write-Back & Invalidate by address
#define	CACHEOP_HIT_WB	(6<<2)	// Write-Back		   by address
#define	CACHEOP_LOCK	(6<<2)	// Fetch & Lock		   by address

#define	CACHEOP_D_HIT_INV	(CACHEOP_LD | CACHEOP_HIT_INV)
#define	CACHEOP_D_HIT_WB	(CACHEOP_LD | CACHEOP_HIT_WB)
#define	CACHEOP_D_HIT_WBI	(CACHEOP_LD | CACHEOP_HIT_WBI)
#define	CACHEOP_D_IDX_LTA	(CACHEOP_LD | CACHEOP_IDX_LTA)
#define	CACHEOP_D_IDX_STA	(CACHEOP_LD | CACHEOP_IDX_STA)
#define	CACHEOP_D_IDX_WBI	(CACHEOP_LD | CACHEOP_IDX_WBI)
#define	CACHEOP_D_LOCK		(CACHEOP_LD | CACHEOP_LOCK)
#define	CACHEOP_I_FILL		(CACHEOP_LI | CACHEOP_FILL)
#define	CACHEOP_I_HIT_INV	(CACHEOP_LI | CACHEOP_HIT_INV)
#define	CACHEOP_I_IDX_INV	(CACHEOP_LI | CACHEOP_IDX_INV)
#define	CACHEOP_I_IDX_LTA	(CACHEOP_LI | CACHEOP_IDX_LTA)
#define	CACHEOP_I_IDX_STA	(CACHEOP_LI | CACHEOP_IDX_STA)
#define	CACHEOP_I_LOCK		(CACHEOP_LI | CACHEOP_LOCK)

#endif
