#include <arch/arch.h>
#include <arch/asm-offsets.h>


#define INVAL_SYNC	0
#define INVAL_IRQ	1
#define INVAL_FIQ	2
#define INVAL_ERROR	3

#define INVAL_EL1T	0
#define INVAL_EL1H	4
#define INVAL_EL0_64	8
#define INVAL_EL0_32	12

.macro	regs_save
	sub	sp, sp, #EFRAME_SIZE
	stp	 x0,  x1, [sp, #8 *  0]
	stp	 x2,  x3, [sp, #8 *  2]
	stp	 x4,  x5, [sp, #8 *  4]
	stp	 x6,  x7, [sp, #8 *  6]
	stp	 x8,  x9, [sp, #8 *  8]
	stp	x10, x11, [sp, #8 * 10]
	stp	x12, x13, [sp, #8 * 12]
	stp	x14, x15, [sp, #8 * 14]
	stp	x16, x17, [sp, #8 * 16]
	stp	x18, x19, [sp, #8 * 18]
	stp	x20, x21, [sp, #8 * 20]
	stp	x22, x23, [sp, #8 * 22]
	stp	x24, x25, [sp, #8 * 24]
	stp	x26, x27, [sp, #8 * 26]
	stp	x28, x29, [sp, #8 * 28]

	add	x3, sp, #EFRAME_SIZE	// original SP
	stp	x30, x3, [sp, #EFRAME_LR]

	mrs	x4, elr_el1
	mrs	x5, spsr_el1
	stp	x4, x5, [sp, #EFRAME_PC]

	/*
	 * The following registers contains:
	 * - r3: original SP
	 * - r4: original PC
	 * - r5: original PSTATE
	 * - sp: the saved frame
	 */
.endm

.macro	regs_restore
	ldp	x4, x5, [sp, #EFRAME_PC]
	msr	elr_el1, x4
	msr	spsr_el1, x5

	ldp	 x0,  x1, [sp, #8 *  0]
	ldp	 x2,  x3, [sp, #8 *  2]
	ldp	 x4,  x5, [sp, #8 *  4]
	ldp	 x6,  x7, [sp, #8 *  6]
	ldp	 x8,  x9, [sp, #8 *  8]
	ldp	x10, x11, [sp, #8 * 10]
	ldp	x12, x13, [sp, #8 * 12]
	ldp	x14, x15, [sp, #8 * 14]
	ldp	x16, x17, [sp, #8 * 16]
	ldp	x18, x19, [sp, #8 * 18]
	ldp	x20, x21, [sp, #8 * 20]
	ldp	x22, x23, [sp, #8 * 22]
	ldp	x24, x25, [sp, #8 * 24]
	ldp	x26, x27, [sp, #8 * 26]
	ldp	x28, x29, [sp, #8 * 28]

	ldr	x30, [sp, #EFRAME_LR]
	add	sp, sp, #EFRAME_SIZE	// == ldr sp, [sp, #FRAME_SP]
.endm


#ifdef CONFIG_INTERRUPT_STACK
.macro	get_irq_stack
	mov	x19, sp			// preserve the original sp

	// WARNING: nested interrupts won't be supported
	//	    (at least not without some chnages)
#ifndef CONFIG_SMP
	ldr	x0, =__interrupt_stack	+ CONFIG_INTERRUPT_STACK_SIZE
	mov	sp, x0
#else
#error	"TODO: percpu IRQ stacks"
#endif

	// mark the end of the frame chain
	stp     xzr, xzr, [sp, #16]!
.endm

.macro	put_irq_stack
	mov	sp, x19			// restore the original sp
.endm
#else
.macro	get_irq_stack
.endm

.macro	put_irq_stack
.endm
#endif


////////////////////////////////////////////////////////////////////////////////

.align 6
LOCAL(except_return):
	regs_restore
	eret
ENDFUN(except_return)

.align 6
LOCAL(except_invalid):
	// FIXME: do something ?
	b	except_return
ENDFUN(except_invalid)

.align 6
LOCAL(except_el1_sync):
	mov	x0, sp
	mrs	x1, far_el1
	mrs	x2, esr_el1
	bl	__sync_handler
	b	except_return
ENDFUN(except_el1_sync)

.align 6
LOCAL(except_el1_irq):
	mov	x0,  sp			// eframe args to handler

	get_irq_stack
	bl	mach_irq_handler
	put_irq_stack

	b	except_return
ENDFUN(except_el1_irq)


// Too bad: the space between the vectors (0x80)
// is too small to put the complete exception routine here
// => we have to branch where the real job is done
.macro	vector, name
	.align 7
	LOCAL(vector_\name):
	regs_save
	b	except_\name
.endm

.macro	vector_invalid, name, reason
	.align 7
	LOCAL(vector_invalid_\name):
	regs_save
	mov	x4, #\reason
	b	except_invalid
.endm

.section ".vectors","ax"
.align 11
GLOBAL(__exception_vectors):
// EL1t
	vector_invalid	el1t_sync,    INVAL_EL1T|INVAL_SYNC
	vector_invalid	el1t_irq,     INVAL_EL1T|INVAL_IRQ
	vector_invalid	el1t_fiq,     INVAL_EL1T|INVAL_FIQ
	vector_invalid	el1t_serror,  INVAL_EL1T|INVAL_ERROR

// EL1h
	vector		el1_sync
	vector		el1_irq
	vector_invalid	el1h_fiq,     INVAL_EL1H|INVAL_FIQ
	vector_invalid	el1h_serror,  INVAL_EL1H|INVAL_ERROR

// EL0-aarch64
	vector_invalid	el064_sync,   INVAL_EL0_64|INVAL_SYNC
	vector_invalid	el064_irq,    INVAL_EL0_64|INVAL_IRQ
	vector_invalid	el064_fiq,    INVAL_EL0_64|INVAL_FIQ
	vector_invalid	el064_serror, INVAL_EL0_64|INVAL_ERROR

// EL0-aarch32
	vector_invalid	el032_sync,   INVAL_EL0_32|INVAL_SYNC
	vector_invalid	el032_irq,    INVAL_EL0_32|INVAL_IRQ
	vector_invalid	el032_fiq,    INVAL_EL0_32|INVAL_FIQ
	vector_invalid	el032_serror, INVAL_EL0_32|INVAL_ERROR
ENDFUN(__exception_vectors)
