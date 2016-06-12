#include <arch/assembly.h>
#include <arch/memory.h>
#include <arch/arch.h>


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

	// TODO: drop to EL1 if needed

	ldr	x5, =__exception_vectors
	msr	vbar_el1, x5

#ifdef	CONFIG_FIXED_STACKS
	// initialize stack
	ldr     x0, =init_thread + (1 << CONFIG_FIXED_STACKS_SHIFT) - 16
	mov	sp, x0
#else
	ldr     x0, =__startup_stack + CONFIG_STARTUP_STACK_SIZE - 16
	mov	sp, x0
#endif

	// mark the end of the frame chain
	stp	xzr, xzr, [sp, #16]!
	mov	fp, #0
	b	_os_start

////////////////////////////////////////////////////////////////////////////////
