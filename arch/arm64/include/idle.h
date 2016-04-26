#ifndef	_ARCH_IDLE_H_
#define	_ARCH_IDLE_H_


static void mach_idle(void)
{
	__asm__ __volatile__ ("wfi");
}

#endif
