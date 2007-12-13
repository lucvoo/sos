#include "arch/arch.h"
#include "arch/mach/platform_setup.S"
#include "arch/memory.h"


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
	ldr	sp, =(KERNEL_RAM_PADDR)

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

	ldr	lr, =post_mmu		@ load address to jump when MMU is set
	mov	r1, #0

	MMU_SETUP

	// Set domain access
	ldr	r2,=0xffffffff
	mcr	p15, 0, r2, c3, c0, 0	@ load domain access register

	mcr	p15, 0, r0, c2, c0, 0	@ load page table pointer
	mcr	p15, 0, r1, c8, c7, 0	@ flush TLB

	mcr	p15, 0, r4, c1, c0, 0	@ write control reg
	mrc	p15, 0, r3, c0, c0, 0	@ read id reg
	nop
	nop
	mov pc, lr

sctlr_mask:
	.word	SCTLR_CLR	// To be defined in platform_setup.S
	.word	SCTLR_SET

	.ltorg

post_mmu:
	// initialize interrupt/exception environments
#ifdef	CONFIG_FIXED_STACKS
	// initialize stack
	ldr     sp,=init_thread + (1 << CONFIG_FIXED_STACKS_SHIFT)
#else
	ldr     sp,=__startup_stack + CONFIG_STARTUP_STACK_SIZE
#endif
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_IRQ)
	msr     cpsr_c, r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_UND)
	msr     cpsr_c, r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE

	// initialize CPSR (machine state register)
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_SVC)
	msr     cpsr_c, r0

	bl	__high_vectors
	b	_os_start


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
	mrc	p15, 0, r1, c5, c0, 0		@ get FSR
	mrc	p15, 0, r0, c6, c0, 0		@ get FAR
	b	vector_p_abt
	.size   vector_p_abt, .-vector_p_abt

vector_d_abt:
	mrc	p15, 0, r1, c5, c0, 0		@ get FSR
	mrc	p15, 0, r0, c6, c0, 0		@ get FAR
	b	vector_d_abt
	.size   vector_d_abt, .-vector_d_abt

vector_fiq:
	b	vector_fiq
	.size   vector_fiq, .-vector_fiq
