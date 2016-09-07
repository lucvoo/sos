#include <arch/regs-copro.h>
#include <arch/cachesize.h>
#include <arch/assembly.h>
#include <arch/cacheop.h>
#include <arch/memory.h>


#if ICACHE_SIZE != DCACHE_SIZE
#error	"icache & dcache must have the same size"
#endif
#if CACHE_LI_LINESIZE != CACHE_LD_LINESIZE
#error	"icache & dcache must have the same linesize"
#endif

.section .init.text.cache_l1_init,"ax",@progbits

ENTRY(__cache_l1_init, global)
	.set noreorder


	li	t1, KSEG0_BASE
	li	t2, KSEG0_BASE | DCACHE_SIZE
	mtc0	$0, c0_taglo
1:	cache	CACHEOP_D_IDX_STA, 0(t1)
	cache	CACHEOP_I_IDX_STA, 0(t1)
	bne	t1, t2, 1b
	addiu	t1, t1, CACHE_LD_LINESIZE

	jr	ra
ENDFUN(__cache_l1_init)
