#include <arch/asm-offsets.h>
#include <arch/regs-copro.h>
#include <arch/assembly.h>
#include <arch/memory.h>
#include <arch/cacheop.h>


// FIXME: get this from the config register?
#define	CACHE_SIZE	(32*1024)
#define	CACHE_LINESIZE	32


.extern	__smp_sec_cpu_thread

.section .init.text.smp_entry,"ax",@progbits
.balign	0x10000

ENTRY(__jz4780_smp_sec_cpu_entry, global)
	.set noreorder
	mtc0	$0, c0_cause

	li	t0, ST0_CU0
	mtc0	t0, c0_status

	// Cache setup
	li	t1, KSEG0_BASE
	ori	t2, t1, CACHE_SIZE
	mtc0	$0, c0_taglo
1:	cache	CACHEOP_I_IDX_STA, 0(t1)
	cache	CACHEOP_D_IDX_STA, 0(t1)
	bne	t1, t2, 1b
	addiu	t1, t1, CACHE_LINESIZE

	// KSEG0 cache coherency attribute
	mfc0	t3, c0_config
	ori	t3, t3, CACHE_COHERENCY_CACHEABLE
	mtc0	t3, c0_config

	// pagemask
	mtc0	$0, c0_pagemask

	// initialize exception vectors
	la	t4, __ex_base
	mtc0	t4, c0_ebase

	// load gp & sp
	la	t5, __smp_sec_cpu_thread
	lw	gp, 0(t5)
	lw	sp, THR_CPU_SP(gp)

	la	t6, __smp_start
	jr.hb	t6
ENDFUN(__jz4780_smp_sec_cpu_entry)
