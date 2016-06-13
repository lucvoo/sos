#include <arch/assembly.h>
#include <arch/arch.h>
#include <arch/el3.h>
#include <arch/el2.h>
#include <arch/timer.h>


.macro switch_el3_el1
	adr	lr, 1f
	msr	elr_el3, lr
	mov	x5, #(PSR_DAIF|PSR_EL1|PSR_SPh)
	msr	spsr_el3, x5

	mrs	x6, sctlr_el1
	bic	x6, x6, #SCTLR_WXN
	msr	sctlr_el1, x6

	mov	x7, #(SCR_RW|SCR_ST|SCR_FIQ)
	msr	scr_el3, x7

	mrs	x8, hcr_el2
	bic	x8, x8, #HCR_RW
	msr	hcr_el2, x8


	mrs	x5, cptr_el2
	bic	x5, x5, #CPTR_TCPAC
	bic	x5, x5, #CPTR_TTA
	bic	x5, x5, #CPTR_TFP
	msr	cptr_el2, x5

	mov	x6, #(CNTHCTL_EL1PCTEN|CNTHCTL_EL1PCEN)
	msr	cnthctl_el2, x6

	msr	cntvoff_el2, xzr

	msr	vttbr_el2, xzr

	isb
	eret
1:
.endm
