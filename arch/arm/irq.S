#include <arch/arch.h>
#include <arch/mach/irq.S>
#include <arch/asm-offsets.h>

	.text
	.align	2
	.type	__irq_svc, %function

__irq_svc:
		@ we are in SVC mode with interrupts disabled
		@ r0 point to a memory zone containing the old r0, the return address and PSR

		@ reserve a stack frame and save r1-r12 into SP+4, SP+8 ...
		sub     sp, sp, #S_FRAME_SIZE
		stmib   sp, {r1 - r12}

		@ load r1, r2, r3 from the IRQ stack
		@   r1 := 'old_r0''
		@   r2 := lr_IRQ
		@   r3 := spsr_IRQ
		ldmia   r0, {r1 - r3}

		@ save the 'real' r0 copied from the IRQ stack into SP+0
		str     r1, [sp]

		@ save the remaining register with a copy of the IRQ stack:
		@   r0 := sp_SVC
		@   r1 := lr_SVC
		@   r2 := lr_IRQ
		@   r3 := spsr_IRQ
		@   r4 := 'orig_r0' ???
		@ this is equivalent to doing:
		@	str	sp_SVC,	[sp, #S_SP]
		@	str	lr_SVC, [sp, #S_LR]
		@	str	lr_IRQ, [sp, #S_PC]
		@	str	spsr_IRQ, [sp, #S_PSR]
		@	str	#-1, [sp, #S_OLD_R0]
		add     r0, sp, #S_FRAME_SIZE
		mov     r1, lr
		mov     r4, #-1
		add     r5, sp, #S_SP
		stmia   r5, {r0 - r4}
		@ now sp point to a full stack frame: r0-r12, sp_SVC, lr_SVC, lr_IRQ, SPSR_IRQ, -1 for old_r0

		@ platform specific macro testing if there is any intr &
		@ returning (if any) the irq number in r0 (r5,r6,lr can be used as scratch)
1:
		get_irqnr r0, r6, r5, lr

		@ while there is an interrupt call __do_IRQ(irq, frame):
		movne   r1, sp
		adrne	lr, 1b		// load lr with a label
		bne	__do_IRQ

		@ this is the place where we should do a scheduling

		@ restore the original spsr:
		ldr     r0, [sp, #S_PSR]
		msr     spsr_cxsf, r0

		@ return from interrupt processing: reload all the register and set cpsr from spsr
		@ note: we have left from IRQ mode already in vector_irq, this is thus not a RFI
		ldmia   sp, {r0 - pc}^

	.size   __irq_svc, .-__irq_svc


	.text
	.align	2
	.type	__irq_invalid, %function
__irq_invalid:
		b	__irq_invalid	// FIXME

	.size   __irq_invalid, .-__irq_invalid


	.text
	.align	2
	.global	vector_irq
	.type	vector_irq, %function

vector_irq:
		@ on the IRQ stack we save r0, lr_IRQ, spsr_IRQ
		@ r0 is then loaded with the address of this stack
		@ then we atomically jump to the IRQ routine while switching to SVC mode

		@ adjust lr_IRQ
		sub     lr, lr, #4

		@ save r0, lr_IRQ & spsr_IRQ on IRQ stack
		stmia   sp, {r0, lr}
		mrs     lr, spsr
		str     lr, [sp, #8]

		@ set spsr_IRQ to SVC MODE
		mrs     r0, cpsr
		eor     r0, r0, #(PSR_MODE_IRQ ^ PSR_MODE_SVC)
		msr     spsr_cxsf, r0

		@ load r0 with sp_IRQ
		mov     r0, sp

		@ use spsr_IRQ & 0xf to determine from wich mode we were
		and     lr, lr, #15

		@ use this to load lr with the irq routine adapted this mode
		@ (here we are only interrested to __irq_svc & __irq_usr)
		ldr     lr, [pc, lr, lsl #2]
		@ magic: the table is just 2 inst after
		@ 	 so pc refer to this address!

		@ jump to this address and switch to SVC mode
		movs    pc, lr

		.long	__irq_invalid			@  0  (USR_26 / USR_32)
		.long	__irq_invalid			@  1  (FIQ_26 / FIQ_32)
		.long	__irq_invalid			@  2  (IRQ_26 / IRQ_32)
		.long	__irq_svc			@  3  (SVC_26 / SVC_32)
		.long	__irq_invalid			@  4
		.long	__irq_invalid			@  5
		.long	__irq_invalid			@  6
		.long	__irq_invalid			@  7
		.long	__irq_invalid			@  8
		.long	__irq_invalid			@  9
		.long	__irq_invalid			@  a
		.long	__irq_invalid			@  b
		.long	__irq_invalid			@  c
		.long	__irq_invalid			@  d
		.long	__irq_invalid			@  e
		.long	__irq_invalid			@  f

		.size   vector_irq, .-vector_irq
