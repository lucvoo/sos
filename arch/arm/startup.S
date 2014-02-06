#include "arch/arch.h"
#include "arch/mach/platform_setup.S"
#include "arch/memory.h"


	.section	".init.text","ax"
	.global _os_startup

_os_startup:
	// * We should be loaded at some multiple of 0x8000 (of a 1MB boundary)
	// * We're in supervisor mode
	// * MMU should be disabled
	// * interrupts should be disabled
	// * r0 should be zero
	// * r1 contains the machine or platform ID
	// * r2 contains the address of the ATAGs or the binary Device Tree
	// We must not corrupt r1 & r2

	//
	// TODO: insure that we're in supervisor mode with interrupts disabled

	// Calculate the physical (load) address
	adr	ip, _os_startup		@ load/phys address of X
	ldr	r3, =_os_startup	@ corresponding virtual address
	sub	r3, r3, ip		@ virt(x) - phys(x)
	rsb	r3, r3, #VIRT_ADDR	@ virt(0) - (virt(x) - phys(x)) = phys(0)

	add	sp, r3, #TEXT_OFFSET	@ set a temporary stack just below our load address

	// r1 = mach ID,
	// r2 = ATAGs/DT, 
	// r3 = PHYS_ADDR

	// platform setup, initialize MMU, ...
	// return:
	//	r5&r6: SCTLR's bits to clear & set

	ldr	r5, =SCTLR_CLR		// To be defined in platform_setup.S
	ldr	r6, =SCTLR_SET
	PLATFORM_SETUP

	// Prepare SCTLR's value
	mrc	p15, 0, r4, c1, c0	@ get control register
	bic	r4, r4, r5
	orr	r4, r4, r6
	// r4 = value to load into SCTLR

	ldr	lr, =post_mmu		@ load address to jump when MMU is set
	mov	r1, #0

	MMU_SETUP

	// Set domain access
	ldr	r5,=0xffffffff
	mcr	p15, 0, r5, c3, c0, 0	@ load domain access register

	mov	r6, r3			@ just below our load address
	mcr	p15, 0, r6, c2, c0, 0	@ load TTBR0

	mcr	p15, 0, r1, c8, c7, 0	@ flush TLB

	mcr	p15, 0, r4, c1, c0, 0	@ write control reg
	mrc	p15, 0, r3, c0, c0, 0	@ read id reg
	nop
	nop

	mov pc, lr

2:	.long	.			@ let store our own (virtual) address

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
