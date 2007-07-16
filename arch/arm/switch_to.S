#include <arch/asm-offsets.h>

/*
 * Register switch for ARMv3 and ARMv4 processors
 *	r0 = previous thread_info,
	r1 = next thread_info
 *   returns:
 *	r0 = previous thread_info
 * previous and next are guaranteed not to be the same.
 */
	.text
	.align	2
	.global	__switch_to
	.type	__switch_to, %function

__switch_to:
	// r0-r3 & ip are not saved as they are call clobered
	add	ip, r0, #TI_CPU_SAVE
	stmia	ip!, {r4 - sl, fp, sp, lr}	@ Store most regs

	add	r1, r1, #TI_CPU_SAVE
	ldmia	r1,  {r4 - sl, fp, sp, pc}	@ Load all regs saved previously

	.size   __switch_to, .-__switch_to
