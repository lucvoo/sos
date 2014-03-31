#include "arch/arch.h"
#include "arch/mach/platform_setup.S"

	.section ".vectors","ax"

__exception_handlers:
	ldr	pc, =vector_reset	// should never be reached
	ldr	pc, =vector_undef
	ldr	pc, =vector_swint
	ldr	pc, =vector_p_abt
	ldr	pc, =vector_d_abt
	.word	0			// unused
	ldr	pc, =vector_irq
	ldr	pc, =vector_fiq

	.size __exception_handlers, . - __exception_handlers


	.section	".init.text","ax"
	.global _os_startup

_os_startup:
	// platform setup, initialize MMU, ...
	// return:
	//	r0: physical address of the L1 MMU table
	PLATFORM_SETUP

	// Prepare value of SCTLR register
	adr	r1, sctlr_mask
	ldmia   r1, {r2, r3}	
	mrc	p15, 0, r4, c1, c0              @ get control register
	bic	r4, r4, r2
	orr	r4, r4, r3

	// Set domain access
	ldr	r1,=0xffffffff		// FIXME
	mcr	p15, 0, r1, c3, c0, 0	@ load domain access register

	adr	lr, 1f			@ load address to jump when MMU is set

	mov	r1, #0
	mcr	p15, 0, r1, c7, c7, 0	@ invalidate caches
	mcr	p15, 0, r1, c8, c7, 0	@ flush TLB (v4)

	mcr	p15, 0, r0, c2, c0, 0	@ load page table pointer

	mcr	p15, 0, r4, c1, c0, 0	@ write control reg
	mrc	p15, 0, r3, c0, c0, 0	@ read id reg
	nop
	nop
	mov pc, lr

1:
	// initialize interrupt/exception environments
	ldr     sp,=__startup_stack + CONFIG_STARTUP_STACK_SIZE - 8
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_IRQ)
	msr     cpsr,r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE - 8
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_UND)
	msr     cpsr,r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE - 8

	// initialize CPSR (machine state register)
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_SVC)
	msr     cpsr,r0

#ifdef	CONFIG_FIXED_STACKS
	// initialize stack
	ldr     sp,=init_thread + (1 << CONFIG_FIXED_STACKS_SHIFT) - 8
#else
#error	Init stack not initialized!
#endif

	bl	__high_vectors
	b	_os_start


#if defined(CONFIG_ARM720) || defined(CONFIG_ARM7TDMI)
// FIXME: move this in a separate header file
	//  R
	// .RVI ZFRS BLDP WCAM
	// ..1. 1001 ..11 1101
sctlr_mask:
	.word	SCTLR_F|SCTLR_R|SCTLR_A
	.word	SCTLR_V|SCTLR_Z|SCTLR_S|SCTLR_D|SCTLR_P|SCTLR_W|SCTLR_C|SCTLR_M
#endif
	.ltorg


	.text

vector_reset:
	b	vector_reset
	.size   vector_reset, .-vector_reset

vector_undef:
	b	vector_undef
	.size   vector_undef, .-vector_undef

vector_swint:
	b	vector_swint
	.size   vector_swint, .-vector_swint

vector_p_abt:
	b	vector_p_abt
	.size   vector_p_abt, .-vector_p_abt

vector_d_abt:
	b	vector_d_abt
	.size   vector_d_abt, .-vector_d_abt

vector_fiq:
	b	vector_fiq
	.size   vector_fiq, .-vector_fiq

	.ltorg
