/*
 * Register switch for ARMv3 and ARMv4 processors
 * r0 = previous thread_info, r1 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
	.text
	.align	2
	.global	__switch_to
	.type	__switch_to, %function

__switch_to:
	// the registers are always at ofset 0
	add	ip, r0, #0			@ FIXME: why ???
	stmia	ip!, {r4 - sl, fp, sp, lr}	@ Store most regs
	ldmia	r1,  {r4 - sl, fp, sp, pc}	@ Load all regs saved previously

	.size   __switch_to, .-__switch_to
