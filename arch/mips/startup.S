#include <arch/assembly.h>
#include <arch/regs-copro.h>


// FIXME: this is cpu/subarch specific
#define	ST0_CLRMSK	(ST0_KSU|ST0_BEV|ST0_ERL|ST0_EXL|ST0_IE|ST0_IMASK)
#define	ST0_SETMSK	0

	.section	".startup.text","ax"
	.global	_os_startup
	.type	_os_startup, @function
	.ent	_os_startup

_os_startup:
#if (CONFIG_TEXT_OFFSET != CONFIG_LOAD_OFFSET)
	la	s0, _os_startup - CONFIG_TEXT_OFFSET + CONFIG_LOAD_OFFSET
	la	s2, _os_startup
	la	s4, __data_end

1:
	lw	s1, 0(s0)
	sw	s1, 0(s2)
	synci	    0(s2)

	addiu	s0,s0,4
	addiu	s2,s2,4
	bne	s2,s4,1b
	 nop
#endif

	// end of PIC code
	la	t0, 1f
	jr	t0
1:

	// init the status register
	mfc0	t0, c0_status
	or	t0, t0, ST0_CLRMSK|ST0_SETMSK
	xor	t0, t0, ST0_CLRMSK
	mtc0	t0, c0_status
	ehb

	// clear cp0 context
	mtc0	$0, c0_context

        // initialize stack
#ifdef	CONFIG_THREAD_STACK
	la	sp, init_thread + (1 << CONFIG_THREAD_STACK_SHIFT) - 4
#else
	la	sp, __startup_stack + CONFIG_STARTUP_STACK_SIZE
#endif

	// initialize exception vectors
	la	t0, __ex_base
	mtc0	t0, c0_ebase

	// continue with C code
	j	_os_start

	.end	_os_startup
	.size	_os_startup, .-_os_startup
