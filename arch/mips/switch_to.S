#include <arch/assembly.h>
#include <arch/regs-copro.h>
#include <arch/asm-offsets.h>


// struct thread* __switch_to(struct thread* prev, struct thread* next);
EXTFUN(__switch_to, 0, ra)
	// save prev's context
	sw	s0, THR_CPU_SREGS+0*4(a0)
	sw	s1, THR_CPU_SREGS+1*4(a0)
	sw	s2, THR_CPU_SREGS+2*4(a0)
	sw	s3, THR_CPU_SREGS+3*4(a0)
	sw	s4, THR_CPU_SREGS+4*4(a0)
	sw	s5, THR_CPU_SREGS+5*4(a0)
	sw	s6, THR_CPU_SREGS+6*4(a0)
	sw	s7, THR_CPU_SREGS+7*4(a0)
	sw	sp, THR_CPU_SP(a0)
	sw	fp, THR_CPU_FP(a0)
	sw	ra, THR_CPU_RA(a0)

	// save the float registers if needed


	// we're switching here


	// restore the floats registers if needed

	// restore next's context
	lw	ra, THR_CPU_RA(a1)
	lw	fp, THR_CPU_FP(a1)
	lw	sp, THR_CPU_SP(a1)
	lw	s7, THR_CPU_SREGS+7*4(a1)
	lw	s6, THR_CPU_SREGS+6*4(a1)
	lw	s5, THR_CPU_SREGS+5*4(a1)
	lw	s4, THR_CPU_SREGS+4*4(a1)
	lw	s3, THR_CPU_SREGS+3*4(a1)
	lw	s2, THR_CPU_SREGS+2*4(a1)
	lw	s1, THR_CPU_SREGS+1*4(a1)
	lw	s0, THR_CPU_SREGS+0*4(a1)

	move	v0, a0
	jr	ra
	
ENDFUN(__switch_to)
