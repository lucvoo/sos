#include <arch/regs-copro.h>
#include <arch/assembly.h>
#include <arch/asm-offsets.h>
#include <arch/eframe.S>

/*
 * 			ROM entry point		RAM entry point
 * 			Status.BEV == 1		Status.BEV == 0
 *
 * Reset & NMI		0xBFC00000		-
 * Simple TLB refill	0xBFC00200		BASE + 0x000
 * Cache error		0xBFC00300		BASE + 0x100 (forced into KSEG1)
 * Generic		0xBFC00380		BASE + 0x180
 * Interrupt Special	0xBFC00400		BASE + 0x200 (Cause.IV == 1)
 */

.macro	ex_entry
	save_regs_all

	// disable interrupts & errors & force kernel mode
	ori	a2, (ST0_IE|ST0_KSU|ST0_UM)
	xori	a2, (ST0_IE|ST0_KSU|ST0_UM)
	mtc0	a2, c0_status
	ehb
.endm


.align	12	// EBase must be 4K aligned

.global	__ex_base
ENTRY(__ex_TLB_refill, local)
__ex_base:

	// FIXME/TODO

	eret
ENDFUN(__ex_TLB_refill)


.align	8
ENTRY(__ex_cache_error, local)

	// FIXME/TODO

	eret
ENDFUN(__ex_cache_error)


.align	7
ENTRY(__ex_generic, local)
	mfc0	k0, c0_cause
	andi	k0, k0, CAUSE_CODE_MSK

	lw	k1, __exception_vectors(k0)
	jr	k1
ENDFUN(__ex_generic)


.align	7
ENTRY(vec_irq, local)

	// FIXME/TODO

	eret
ENDFUN(vec_irq)



ENTRY(ret_from_exception, local)
	restore_regs_all

	eret
ENDFUN(ret_from_exception)


ENTRY(vec_unhandled, local)
	ex_entry

	move	a0, sp
	la	ra, ret_from_exception
	j	__handle_exceptions
ENDFUN(vec_unhandled)


        .section        .data,"aw",@progbits
        .align  2
        .type   __exception_vectors, @object
        .size   __exception_vectors, 128
__exception_vectors:
        .word	vec_unhandled	// int
        .word	vec_unhandled	// tlbm
        .word	vec_unhandled	// tlbl
        .word	vec_unhandled	// tlbs
        .word	vec_unhandled	// adel
        .word	vec_unhandled	// ades
        .word	vec_unhandled	// ibe
        .word	vec_unhandled	// dbe
        .word	vec_unhandled	// sys
        .word	vec_unhandled	// bp
        .word	vec_unhandled	// ri
        .word	vec_unhandled	// cpu
        .word	vec_unhandled	// ov
        .word	vec_unhandled	// tr
        .word	vec_unhandled
        .word	vec_unhandled	// fpe
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled	// c2e
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled	// mdmx
        .word	vec_unhandled	// watch
        .word	vec_unhandled	// mcheck
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled
        .word	vec_unhandled	// cacheerr
        .word	vec_unhandled
