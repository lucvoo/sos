#include <arch/arch.h>
#include <arch/asm-offsets.h>
#include <arch/cp15.h>


.macro	vector_entry, name, adjust=0
	.text
	.align	5
	.global	vector_\name
	.type	vector_\name, %function

vector_\name:
    .if \adjust
	sub	lr, lr, #\adjust			// adjust the prefered return address
    .endif
	srsdb	sp!, #PSR_MODE_SVC!		// Store adjusted lr and spsr to SVC mode stack
	cps	#PSR_MODE_SVC			// Switch to SVC mode, keeping all interrupts disabled

	// stack layout:
	//	|      |	SP -> |  PC  |0	// Return Address
	//	|      |  ==>	      | SPSR |1 // Saved PSR
	// SP ->|      | 0	      |      |2

	// Finish the fill the struct eframe
	sub	sp, sp, #(S_FRAME_SIZE-8)	// -8 because lr & psr have been just saved
	stmia	sp, {r0-r12,lr}			// store the registers

	add	r0, sp, #(S_FRAME_SIZE)
	str	r0, [sp, #S_SP]

	
	// stack layout:
	//			SP -> |  R0  |0
	//			      | .... |
	//			      |  R12 |12
	//			      |  LR  |13
	//			      |  SP  |14
	//SP -> |  PC  |	      |  PC  |15
	//      | SPSR |	      | SPSR |16
	//      |      |	      |      |

	// Remark:
	//	* Only r0-r3, ip & lr, the call clobered registers, should be saved
	//	  because the C handler will save & restore the others when needed.
	//	  But if we do so, we won't be able to debug what caused the exception

	// FIXME: should insure that the stack is aligned at a doubleword boundary

	mov	r0, sp
.endm

.macro	vector_exit, name
	// Restore the registers and return from exception
	ldmia	sp, {r0-r12,lr}			// Restore working registers
	add	sp, sp, #(S_FRAME_SIZE-8)
	rfeia	sp!				// Return

	.size   vector_\name, .-vector_\name
.endm


/*
 * undefined instruction
 */
	vector_entry	undef, 4

	bl	__und_handler			// Branch to C handler

	// Real handler FIXME: move all this to an arch specific C handler
	ldr	r0, [sp, #S_PC]			// Skip the instruction
	add	r0, r0, #4
	str	r0, [sp, #S_PC]

	vector_exit	undef

/*
 * abort
 */
	vector_entry	d_abt, 8

	mrc     DFAR(r1)
	mrc     DFSR(r2)
	bl	dabt_handler			// Branch to C handler

	vector_exit	d_abt


	vector_entry	p_abt, 4

	mrc     IFAR(r1)
	mrc     IFSR(r2)
	bl	pabt_handler			// Branch to C handler

	vector_exit	p_abt

/*
 * interrupt request
 */
	vector_entry	irq, 4
	bl	mach_irq_handler
	vector_exit	irq
