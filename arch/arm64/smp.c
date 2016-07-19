#include <smp.h>
#include <thread.h>


extern struct thread init_thread[];

void __smp_start_idle_thread(void);
void __smp_start_idle_thread(void)
{
	struct thread *idle = &init_thread[__coreid()];

	asm volatile("\n"
	"	mov	sp, %0\n"
	:: "r" (idle->cpu_context.sp)
	);

	set_current_thread(idle);

	asm volatile("\n"
	"	b	__smp_start\n"
	);
	__builtin_unreachable();
}
