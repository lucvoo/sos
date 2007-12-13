	.macro	MMU_SETUP
	// r0 = mmu root page table pointer
	// r1 = 0
	// r4 = control register
	// lr = address to jump after the MMU is enabled

	mcr	p15, 0, r1, c7, c7, 0	@ invalidate caches

1:	mrc	p15, 0, r15, c7, c14, 3	@ test,clean,invalidate D caches
	bne	1b

	mcr	p15, 0, r1, c7, c5, 0	@ invalidate I caches
	mcr	p15, 0, r1, c7, c10, 4	@ drain write buffer

	.endm
