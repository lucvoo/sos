#ifndef	_ARCH_IDLE_H_
#define	_ARCH_IDLE_H_


static void mach_idle(void)
{
#ifndef	CONFIG_WAIT_BROKEN
	asm ("wait");
#endif
}

#endif
