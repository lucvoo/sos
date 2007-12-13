	.macro	MMU_SETUP
	// r0 = mmu root page table pointer
	// r1 = 0
	// r4 = control register
	// lr = address to jump after the MMU is enabled
	mcr	p15, 0, r1, c7, c7, 0	@ invalidate caches
	.endm
