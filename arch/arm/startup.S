#include <arch/assembly.h>
#include "arch/arch.h"
#include "arch/mach/platform_setup.S"
#include "arch/memory.h"
#include "arch/hw/mmu.h"
#include "arch/cp15.h"


	.section	".startup.text","ax"
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

	// insure that we're in supervisor mode with interrupts disabled
	msr	cpsr_c, #(PSR_I|PSR_F|PSR_MODE_SVC)

	// FIXME: We should check if the processor ID correspond to the compiled one
	// mrc	MIDR(r3)
	// ...

	// Calculate the physical (load) address
	adr	ip, _os_startup		@ load/phys address of X
	ldr	r3, =_os_startup	@ corresponding virtual address
	sub	r3, r3, ip		@ virt(x) - phys(x)
	rsb	r3, r3, #VIRT_ADDR	@ virt(0) - (virt(x) - phys(x)) = phys(0)

	@ r1: mach ID,
	@ r2: ATAGs/DT, 
	@ r3: PHYS_ADDR

	bl	create_early_pagetable
	mov	fp, r0

	@ fp: address of early pgd

	// set a temporary stack just below our pagetable
	// FIXME: can corrupt ATAG/DTB
	sub	sp, fp, #4

	// platform setup, initialize MMU, ...
	// return:
	//	r5&r6: SCTLR's bits to clear & set

	ldr	r5, =SCTLR_CLR		// To be defined in platform_setup.S
	ldr	r6, =SCTLR_SET
	PLATFORM_SETUP

	// Prepare SCTLR's value
	mrc	SCTLR(r4)       	@ get control register
	bic	r4, r4, r5
	orr	r4, r4, r6
	// r4 = value to load into SCTLR

	ldr	lr, =post_mmu		@ load address to jump when MMU is set

	mov	r0, #0			// often need a register with 0x00000000

	MMU_SETUP

	// Set domain access
	ldr	r5,=0xffffffff
	mcr	DACR(r5)		@ load domain access register

	mcr	ICIALLU(r0)		@ I+BTB cache invalidate
	mcr	TLBIALL(r0)		@ invalidate I + D TLBs

	mov	r5, #TTBCR_N_2G2G
	mcr	TTBCR(r5)		@ Split 2G/2G

	orr	r5, fp, #TTBR_FLAGS	@ but for now use the same table for TTBR0/1
	mcr	TTBR0(r5)
	mcr	TTBR1(r5)

mmu_enable:
	isb
	mcr	SCTLR(r4)       	@ write control reg
	isb
	nop
	nop

	mov pc, lr

2:	.long	.			@ let store our own (virtual) address

	.ltorg

post_mmu:
	// install the exception vectors
#ifdef	CONFIG_HIGH_VECTORS
	ldr	lr, =__vectors_start
	ldmia	lr, {r5, r6, r7, r8, r9, r10, r11, r12}
	ldr	lr, =0xFFFF0000
	stmia	lr, {r5, r6, r7, r8, r9, r10, r11, r12}
#endif
#ifdef	CONFIG_HAS_VBAR
	ldr	r0, =__exception_handlers
	mcr	VBAR(r0)
#endif

#if CONFIG_ARM_ARCH < 6
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
#endif

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

////////////////////////////////////////////////////////////////////////////////
create_early_pagetable:
	@ r1: must be preserved
	@ r2: must be preserved
	@ r3: physical start of RAM

	@ r0: start of page table
	add	r0, r3, #(TEXT_OFFSET - PGD_NBR_ENT*4)

	@ 1) fill all the entries with fault
	@ rem: do this in reverse order to spare a register
	mov	ip, #PGD_NBR_ENT
	mov	r5, #PGD_TYPE_FAULT
1:
	sub	ip, ip, #1
	cmp	ip, #0
	str	r5, [r0, ip, lsl #2]
	bne	1b

	@ 2) create an identity mapping for mmu enable/disable
	ldr	r4, =mmu_enable
	sub	r4, r4, #VIRT_ADDR			@ convert virt -> phys
	add	r4, r4, r3

	ldr	r5, =PGD_INIT_MEM
	lsr	r4, r4, #PGD_SECT_SHIFT
	orr	r5, r5, r4, lsl #PGD_SECT_SHIFT
	str	r5, [r0, r4, lsl #2]

#define IOADDR	0x44e00000
#define	CONFIG_EARLY_DEBUGGING
#ifdef	CONFIG_EARLY_DEBUGGING
	@ 3) create an identity mapping for early early debugging
	// FIXME: for the moment only a single section
	ldr	r4, =IOADDR
	ldr	r5, =PGD_INIT_IO
	lsr	r4, r4, #PGD_SECT_SHIFT
	orr	r5, r5, r4, lsl #PGD_SECT_SHIFT
	str	r5, [r0, r4, lsl #2]
#endif

	@ 4) create the normal memory phys -> virt mapping for the kernel
	// FIXME: for the moment just the first 4MB
	ldr	r5, =PGD_INIT_MEM
	mov	r4, #(VIRT_ADDR >> PGD_SECT_SHIFT)	@ r4 = VADDR >> PGD_SECT_SHIFT
	orr	r5, r5, r3				@ r5 = PADDR | PGD_INIT_MEM
	add	r2, r0, r4, lsl #2			@ r0 = &pgd[vaddr >> PGD_SECT_SHIFT]
	mov	ip, #4					@ ip = 4 << PGD_SECT_SHIFT
1:
	sub	ip, ip, #1
	cmp	ip, #0
	add	r4, r5, ip, lsl #PGD_SECT_SHIFT
	str	r4, [r2, ip, lsl #2]
	bne	1b

	bx	lr
ENDFUN(create_early_pagetable)
////////////////////////////////////////////////////////////////////////////////


	.text

vector_reset:
	b	vector_reset
	.size   vector_reset, .-vector_reset

vector_swint:
	b	vector_swint
	.size   vector_swint, .-vector_swint

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
