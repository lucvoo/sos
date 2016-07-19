#include <smp.h>
#include <thread.h>


extern struct thread init_thread[];

void __smp_start_idle_thread(void) __attribute__((naked));
void __smp_start_idle_thread(void)
{
	struct thread *idle = &init_thread[__coreid()];

	set_current_thread(idle);

	asm("\n"
	"	ldr	sp, %0\n"
	"	b	__smp_start"
	:: "m" (idle->cpu_context.sp));
}
