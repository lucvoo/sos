#include <arch/assembly.h>


.macro	dcache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	ubfx	\tmp, \tmp, #16, #4		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
.endm

/**
 * invalidate a dcache range
 * @start:
 * @size:
 * note: start & size must be cacheline aligned
 */
GLOBAL(__dcache_inv_range):
	dcache_line_size x2, x3
	add	x4, x0, x1
1:	dc	ivac, x0
	add	x0, x0, x2
	cmp	x0, x4
	b.ne	1b
	dsb	sy
	ret
ENDFUN(__dcache_inv_range)
