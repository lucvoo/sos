#include <arch/asm-offsets.h>
#include <arch/regs-copro.h>
#include <arch/assembly.h>
#include <soc/smp.h>


#ifdef CONFIG_MIPS_CM
#define	CFG0_K0_SMP	CFG0_K0_CWB
#else
#define	CFG0_K0_SMP	CFG0_K0_WB
#endif

.section .init.text.smp_entry,"ax",@progbits
.balign	SMP_ENTRY_ALIGN

ENTRY(__smp_entry, global)
	.set noreorder
	mtc0	$0, c0_cause

	li	t0, ST0_CU0
	mtc0	t0, c0_status

	jal	__cache_l1_init
	 nop

#ifdef	CONFIG_MIPS_CM
	// enable coherency
	li	t0, 0xff
	li	t1, 0xbfbfa008	// GCR_CL_COHERENCE
	sw	t0, 0(t1)
	ehb
#endif

	// KSEG0 cache coherency attribute
	mfc0	t3, c0_config
	ori	t3, t3, CFG0_K0_MASK
	xori	t3, t3, CFG0_K0_MASK ^ CFG0_K0_SMP
	mtc0	t3, c0_config
	ehb

	// now that the cache is initialized and
	// coherency is configured
	// we can jump back to kseg0
	la	t0, 1f
	jr	t0
	 nop
1:

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
