#include <arch/assembly.h>
#include <arch/regs-copro.h>


// FIXME: this is cpu/subarch specific
#define	ST0_CLRMSK	(ST0_KSU|ST0_ERL|ST0_EXL|ST0_IE)
#define	ST0_SETMSK	(ST0_CU0)

	.section	".startup.text","ax"
	.global	_os_startup
	.type	_os_startup, @function
	.ent	_os_startup

_os_startup:

	// init the status register
	mfc0	t0, c0_status
	or	t0, t0, ST0_CLRMSK|ST0_SETMSK
	xor	t0, t0, ST0_CLRMSK
	mtc0	t0, c0_status
	ehb


	// end of PIC code
	la	t0, 1f
	jr	t0	
1:

	// clear cp0 context
	mtc0	$0, c0_context

        // initialize stack
#ifdef	CONFIG_FIXED_STACKS
	la	sp, init_thread + (1 << CONFIG_FIXED_STACKS_SHIFT) - 4
#else
	la	sp, __startup_stack + CONFIG_STARTUP_STACK_SIZE
#endif


	// continue with C code
	j	_os_start

	.end	_os_startup
	.size	_os_startup, .-_os_startup
