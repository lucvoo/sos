#define TTB_C				(1 << 0)
#define TTB_S				(1 << 1)
#define TTB_IMP				(1 << 2)
#define TTB_RGN_NC			(0 << 3)
#define TTB_RGN_WBWA		(1 << 3)
#define TTB_RGN_WT			(2 << 3)
#define TTB_RGN_WB			(3 << 3)

#define TTB_FLAGS	TTB_RGN_WBWA

#define PMD_FLAGS	PMD_SECT_WB


.macro	MMU_SETUP

	// r0 = 0
	// r4 = control register
	// lr = address to jump after the MMU is enabled

	mcr	DCCIALL(r0)		@ clean+invalidate D cache
	mcr	CP15DSB(r0)		@ drain write buffer
.endm


	//   AT
	//   FR   EV X F   I D LR
	// 00EE 00EE PUI0 0T0T 4RVI Z0RS B111 1CAM

	// 0000 0000 0000 0101 0000 0000 0111 1010
	// ---- ---- -1-- ---- ---1 1--- ---- -101

#define	SCTLR_CLR	(SCTLR_A)
#define	SCTLR_SET	(SCTLR_U|SCTLR_I|SCTLR_Z|SCTLR_C|SCTLR_M)
