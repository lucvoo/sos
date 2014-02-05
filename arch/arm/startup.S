#include "arch/arch.h"
#include "arch/mach/platform_setup.S"
#include "arch/memory.h"


	.section	".init.text","ax"
	.global _os_startup

_os_startup:
	ldr	sp, =(KERNEL_RAM_PADDR)

	// platform setup, initialize MMU, ...
	// return:
	//	r0: physical address of the L1 MMU table
	PLATFORM_SETUP

	// Prepare SCTLR's value
	ldr	r2, =SCTLR_CLR		// To be defined in platform_setup.S
	ldr	r3, =SCTLR_SET
	mrc	p15, 0, r4, c1, c0	@ get control register
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

	.ltorg

post_mmu:
	// install the exception vectors
#ifdef	CONFIG_HIGH_VECTORS
	bl	__high_vectors
#endif
#ifdef	CONFIG_HAS_VBAR
	ldr	r0, =__exception_handlers
	mcr	p15, 0, r0, c12, c0, 0	@ VBAR
#endif
	// initialize interrupt/exception environments
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_IRQ)
	msr     cpsr_c, r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE

	mov     r0,#(PSR_I|PSR_F|PSR_MODE_UND)
	msr     cpsr_c, r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE

	mov     r0,#(PSR_I|PSR_F|PSR_MODE_ABT)
	msr     cpsr_c, r0
	ldr     sp,=__exception_stack + CONFIG_EXCEPTION_STACK_SIZE

	// initialize CPSR (machine state register)
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_SVC)
	msr     cpsr_c, r0
#ifdef	CONFIG_FIXED_STACKS
	// initialize stack
	ldr     sp,=init_thread + (1 << CONFIG_FIXED_STACKS_SHIFT) - 4
#else
	ldr     sp,=__startup_stack + CONFIG_STARTUP_STACK_SIZE
#endif

	b	_os_start


	.text

vector_reset:
	b	vector_reset
	.size   vector_reset, .-vector_reset

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


	.section ".vectors","ax"
	.align 5

#ifdef	CONFIG_HAS_VBAR
#define	EXCEP(handler)	b	handler
#else
// The handlers will most probably be too far away from the
// exception vector (0x00000000 or 0xffff0000) to be able to use a pc relative branch
#define	EXCEP(handler)	ldr	pc, =handler
#endif
__exception_handlers:
	EXCEP(vector_reset)
	EXCEP(vector_undef)
	EXCEP(vector_swint)
	EXCEP(vector_p_abt)
	EXCEP(vector_d_abt)
	.word	0		// unused
	EXCEP(vector_irq)
	EXCEP(vector_fiq)

	.size __exception_handlers, . - __exception_handlers
