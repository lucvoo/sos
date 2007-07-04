#include "arch/arch.h"

	.section ".vectors","ax"
	.global __exception_handlers

__exception_handlers:
	b	vector_reset
	b	vector_undef
	b	vector_swint
	b	vector_p_abt
	b	vector_d_abt
	.word	0		// unused
	b	vector_irq
	b	vector_fiq

	.size __exception_handlers, . - __exception_handlers


	.text

vector_reset:
	// call MMU, ...
#include "arch/mach/platform_setup.S"

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

.Lstart_hang:
	b	.Lstart_hang



vector_undef:
	b	vector_undef
	.size   vector_undef, .-vector_undef

vector_swint:
	b	vector_swint
	.size   vector_swint, .-vector_swint

vector_p_abt:
	b	vector_p_abt
	.size   vector_p_abt, .-vector_p_abt

vector_d_abt:
	b	vector_d_abt
	.size   vector_d_abt, .-vector_d_abt

vector_fiq:
	b	vector_fiq
	.size   vector_fiq, .-vector_fiq

	.ltorg
