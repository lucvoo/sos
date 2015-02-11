#include <idle.h>
#include <arch/idle.h>
#include <irqflags.h>
#include <sched.h>


static void default_idle(void)
{
	__local_irq_dis();
	if (!need_resched()) {
		mach_idle();
	}
	__local_irq_ena();
}

void __cpu_idle(void)
{
	while (1) {
#define	idle default_idle

		while (!need_resched())
			idle();

		thread_schedule();
	}
}
