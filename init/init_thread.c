#include <thread.h>
#include <init.h>


struct thread init_thread __uninit;

static __init void init_thread_init(void)
{
	dlist_init(&init_thread.run_list);
}
pure_initcall(init_thread_init);
