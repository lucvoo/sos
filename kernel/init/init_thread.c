#include <thread.h>
#include <init.h>
#include <smp.h>


struct thread init_thread[NR_CPUS] __uninit;

#ifndef CONFIG_THREAD_STACK
static void init_thread_init(void)
{
	set_current_thread(&init_thread[0]);
}
pure_initcall(init_thread_init);
#endif
