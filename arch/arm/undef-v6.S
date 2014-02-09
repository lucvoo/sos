#include <arch/arch.h>
#include <arch/asm-offsets.h>

	.text
	.align	5
	.global	vector_undef
	.type	vector_undef, %function

vector_undef:
	sub	lr, lr, #4			// adjust the prefered return address
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

	// Real handler FIXME: move all this to an arch specific C handler
	mov	r0, sp
	bl	und_handler			// Branch to C handler

	ldr	r0, [sp, #S_PC]			// Skip the instruction
	add	r0, r0, #4
	str	r0, [sp, #S_PC]


	// Restore the registers and return from exception
	ldmia	sp, {r0-r12,lr}			// Restore working registers
	add	sp, sp, #(S_FRAME_SIZE-8)
	rfeia	sp!				// Return

	.size   vector_undef, .-vector_undef
