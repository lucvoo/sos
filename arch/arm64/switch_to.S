#include <arch/asm-offsets.h>
#include <arch/assembly.h>

/*
 * task switching:
 *	x0 = pointer to curr thread,
 *	x1 = pointer to next thread.
 *   returns:
 *	x0 = pointer to previous thread
 */
	.text

GLOBAL(__switch_to):
	// store callee-saved registers
	add	x2, x0, #TI_CONTEXT_X0
	stp	x19, x20, [x2, 19 * 8]
	stp	x21, x22, [x2, 21 * 8]
	stp	x23, x24, [x2, 23 * 8]
	stp	x25, x26, [x2, 25 * 8]
	stp	x27, x28, [x2, 27 * 8]
	stp	x29, x30, [x2, 29 * 8]
	mov	x3, sp			// store the stack pointer
	str	x3, [x2, 31 * 8]

	// restore callee-saved registers
	add	x2, x1, #TI_CONTEXT_X0
	ldp	x19, x20, [x2, 19 * 8]
	ldp	x21, x22, [x2, 21 * 8]
	ldp	x23, x24, [x2, 23 * 8]
	ldp	x25, x26, [x2, 25 * 8]
	ldp	x27, x28, [x2, 27 * 8]
	ldp	x29, x30, [x2, 29 * 8]
	ldr	x3, [x2, 31 * 8]	// restore the stack pointer
	mov	sp, x3

	ret
ENDFUN(__switch_to)
