#ifndef	_SMP_INIT_H_
#define	_SMP_INIT_H_


int  __smp_init(void);
void __smp_startup(void) __noreturn;
void __smp_start_mach(void);
void __smp_start(void) __noreturn;

#endif
