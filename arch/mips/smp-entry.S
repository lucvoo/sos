#include <arch/asm-offsets.h>
#include <arch/regs-copro.h>
#include <arch/assembly.h>
#include <soc/smp.h>



.section .init.text.smp_entry,"ax",@progbits
.balign	SMP_ENTRY_ALIGN

ENTRY(__smp_entry, global)
	.set noreorder
	mtc0	$0, c0_cause

	li	t0, ST0_CU0
	mtc0	t0, c0_status

	jal	__cache_l1_init
	 nop

	// KSEG0 cache coherency attribute
	mfc0	t3, c0_config
	ori	t3, t3, CACHE_COHERENCY_CACHEABLE
	mtc0	t3, c0_config

	// pagemask
	mtc0	$0, c0_pagemask

	// initialize exception vectors
	la	t4, __ex_base
	mtc0	t4, c0_ebase

	// get the idle thread
	jal	__smp_init_idle_thread

	// load gp & sp
	move	gp, v0
	lw	sp, THR_CPU_SP(v0)

	la	t6, __smp_start
	jr.hb	t6
ENDFUN(__smp_entry)
