#include <arch/arch.h>
#include <arch/mach/irq.S>
#include <arch/asm-offsets.h>

	.text
	.align	2
	.global	__irq_svc
	.type	__irq_svc, %function

__irq_svc:
		@ reserve a stack frame and save some register
		sub     sp, sp, #S_FRAME_SIZE
		stmib   sp, {r1 - r12}

		@ load r1, r2, r3 from the IRQ stack
		@   r1 := 'old_r0''
		@   r2 := lr_IRQ
		@   r3 := spsr_IRQ
		ldmia   r0, {r1 - r3}

		@ save the the old r0 from the IRQ stack
		str     r1, [sp]

		@ save the remaining register with a copy of the IRQ stack:
		@   r0 := sp_SVC
		@   r1 := lr_SVC
		@   r2 := lr_IRQ
		@   r3 := spsr_IRQ
		@   r4 := 'orig_r0' ???
		add     r0, sp, #S_FRAME_SIZE
		mov     r1, lr
		mov     r4, #-1
		add     r5, sp, #S_SP
		stmia   r5, {r0 - r4}

		@ platform specific macro testing if there is any intr &
		@ returning (if any) the irq number in r0 (r5,r6,lr can be used as scratch)
1:
		get_irqnr r0, r6, r5, lr

		@ while there is an interrupt call asm_do_IRQ(irq, frame):
		movne   r1, sp
		adrne	lr, 1b		// load lr with a label
		bne	asm_do_IRQ

		@ if none or no more:
		@ restore spsr:
		ldr     r0, [sp, #S_PSR]
		msr     spsr_cxsf, r0

		@ return from interrupt:
		ldmia   sp, {r0 - pc}^

	.size   __irq_svc, .-__irq_svc

	.text
	.align	2
	.global	irq_handler
	.type	irq_handler, %function

irq_handler:
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

		@ use lr_IRQ & 0xf to determine from wich mode we were
		and     lr, lr, #15

		@ use this to load lr with the irq routine adapted this mode
		@ (here we are only interrested to __irq_svc & __irq_usr)
		ldr     lr, [pc, lr, lsl #2]
		@ magic: the table is just 2 inst after
		@ 	 so pc refer to this address!

		@ jump to this address and switch to SVC mode
		movs    pc, lr

