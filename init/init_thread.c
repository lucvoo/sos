#include <thread.h>
#include <init.h>


struct thread init_thread __uninit;

static __init void init_thread_init(void)
{
	dlist_init_node(&init_thread.run_list);		// FIXME: shouldn't need to be initialized but .. there is a bug somewhere ...
}
pure_initcall(init_thread_init);
