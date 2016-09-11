#include <arch/asm-offsets.h>
#include <arch/regs-copro.h>
#include <arch/assembly.h>
#include <arch/memory.h>
#include <arch/cacheop.h>
#include <arch/cachesize.h>
#include <soc/smp.h>


#if ICACHE_SIZE != DCACHE_SIZE
#error	"icache & dcache must have the same size"
#endif
#if CACHE_LI_LINESIZE != CACHE_LD_LINESIZE
#error	"icache & dcache must have the same linesize"
#endif


.section .init.text.smp_entry,"ax",@progbits
.balign	SMP_ENTRY_ALIGN

ENTRY(__smp_entry, global)
	.set noreorder
	mtc0	$0, c0_cause

	li	t0, ST0_CU0
	mtc0	t0, c0_status

	// Cache setup
	li	t1, KSEG0_BASE
	ori	t2, t1, DCACHE_SIZE
	mtc0	$0, c0_taglo
1:	cache	CACHEOP_I_IDX_STA, 0(t1)
	cache	CACHEOP_D_IDX_STA, 0(t1)
	bne	t1, t2, 1b
	addiu	t1, t1, CACHE_LD_LINESIZE

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
