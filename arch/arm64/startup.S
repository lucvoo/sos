#include <arch/assembly.h>
#include <arch/asm-offsets.h>
#include <arch/switch_el.S>
#include <arch/memory.h>
#include <arch/stack.h>
#include <arch/arch.h>
#include <arch/mmu.h>
#include <soc/asm.h>


idpgd	.req	x25
sctlr	.req	x26
xpm	.req	x27
fp	.req	x29


	.section	".startup.text","ax"
	.global _os_startup

_os_startup:
	// * EL:	EL1 (or EL2), secure or not
	// * MMU:	disabled
	// * IRQs:	disabled
	// * D-cache:	off
	// * I-cache:	on or off
	// * x0:	address of the Device Tree's blob
	// * x1-x3:	must be preserved (for future use?)

	// DO NOT MODIFY. Image header expected by Linux boot-loaders.
	b	startup				// branch to kernel start, magic
	.long	0				// reserved
	.quad	CONFIG_TEXT_OFFSET		// Image load offset from start of RAM, little-endian
	.quad	0				// Effective size of kernel image, little-endian
	.quad	0				// Informative flags, little-endian
	.quad	0				// reserved
	.quad	0				// reserved
	.quad	0				// reserved
	.byte	0x41				// Magic number, "ARM\x64"
	.byte	0x52
	.byte	0x4d
	.byte	0x64
	.word	0				// reserved

startup:
	// TODO: copy boot args

	// load the (virtual!) address to jump when MMU is set
	ldr	xpm, =post_mmu

startup_cpu:
	startup_cpu	x4, x5

#ifdef	CONFIG_CNTFRQ
	ldr	x6, =CONFIG_CNTFRQ
	msr	cntfrq_el0, x6
#endif

#if defined(CONFIG_ARM64_DROP_EL3_EL1)
	switch_el3_el1
#endif

	ldr	x5, =__exception_vectors
	msr	vbar_el1, x5

#if 1	// early stack for debugging
	ldr     x4, =__startup_stack + STARTUP_STACK_SIZE
	ldr     x5, =(VIRT_ADDR - PHYS_ADDR)
	sub	sp, x4, x5
	stp	xzr, xzr, [sp, #-16]!	// mark the end of the frame chain
	mov	fp, #0
#endif

	// setup MAIR & TCR, init & return the identity table
	bl	__arch_mmu_setup
	mov	idpgd, x0

	// invalidate the page table to avoid any stale entries
	mov	x1, #(PGD_NBR_ENT * 8)
	bl	__dcache_inv_range

	// load the TBRs with the identity mapping
	msr	ttbr0_el1, idpgd	// load TTBR0
	msr	ttbr1_el1, idpgd	// load TTBR1
	isb

	// enable the MMU & the caches
	mrs	sctlr, sctlr_el1
	orr	sctlr, sctlr, SCTLR_I
	orr	sctlr, sctlr, SCTLR_C
	orr	sctlr, sctlr, SCTLR_M
	msr	sctlr_el1, sctlr	// write control reg
	isb

	ic	iallu			// invalidate I-cache
	dsb	nsh
	isb

	br	xpm			// jump to absolute virtual address


post_mmu:
	// From here we run with the MMU on & with virtual addresses

	// setup the normal stack
	ldr     x0, =__startup_stack + STARTUP_STACK_SIZE
	mov	sp, x0
	stp	xzr, xzr, [sp, #-16]!	// mark the end of the frame chain
	mov	fp, #0

	// OK, jump to the C code
	b	_os_start

////////////////////////////////////////////////////////////////////////////////

#ifdef	CONFIG_SMP
	.text

// entry point for secondary CPUs
GLOBAL(__smp_entry):

	// load the (virtual!) address to jump when MMU is set
	ldr	xpm, =post_mmu_smp

	// initialize the cpu
	b	startup_cpu

post_mmu_smp:
	bl	dcache_inval_all
	// From here we run with the MMU on & with virtual addresses

	// need to setup the stack before calling __smp_start
	bl	__smp_init_idle_thread
	ldr	x1, [x0, THR_CTXT_SP]
	mov	sp, x1
	b	__smp_start
ENDFUN(__smp_entry)
#endif

////////////////////////////////////////////////////////////////////////////////
