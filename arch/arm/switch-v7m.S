#include <arch/asm-offsets.h>

/*
 * Register switch for ARMv3 and ARMv4 processors
 *	r0 = prev thread_info,
	r1 = next thread_info
 *   returns:
 *	r0 = prev thread_info
 * previous and next are guaranteed not to be the same.
 */

.syntax unified

	.text
	.align	2
	.global	__switch_to
	.type	__switch_to, %function

__switch_to:
	// r0-r3 & ip are not saved as they are call clobered
	add	ip, r0, #TI_CPU_SAVE
	stmia	ip!, {r4 - fp}		@ Store most regs
	str	sp, [ip], #4
	str	lr, [ip], #4

	add	ip, r1, #TI_CPU_SAVE
	ldmia	ip!,  {r4 - fp}		@ Load all regs saved previously
	ldr	sp, [ip]
	ldr	pc, [ip, #4]!

	.size   __switch_to, .-__switch_to
