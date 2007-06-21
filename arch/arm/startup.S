#include "arch/arch.h"

	.section ".vectors","ax"
	.global __exception_handlers

__exception_handlers:
	b	reset_vector
	b	undefined_instruction
	b	software_interrupt
	b	abort_prefetch
	b	abort_data
	.word	0		// unused
	b	irq_handler
	b	fiq_handler

reset_vector:
	// call MMU, ...
#include "platform_setup.S"

start:
	// move to vectors to their definitive address
	// FIXME

	// initialize interrupt/exception environments
	ldr     sp,=__startup_stack
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_IRQ)
	msr     cpsr,r0
	ldr     sp,=__exception_stack
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_UND)
	msr     cpsr,r0
	ldr     sp,=__exception_stack

	// initialize CPSR (machine state register)
	mov     r0,#(PSR_I|PSR_F|PSR_MODE_SVC)
	msr     cpsr,r0

	// initialize stack
	ldr     sp,=__startup_stack

	bl	__clear_bss

	bl      __invoke_constructors
	// 0	platform hw init
	// ...

	bl	kapi_start
	bl	kapi_scheduler_start

__start_hang:
	b	__start_hang
