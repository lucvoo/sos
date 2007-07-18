#include <thread.h>
#include <init.h>


struct thread init_thread __attribute__((__section__(".init.task")));

static void init_thread_init(void)
{
	dlist_init(&init_thread.run_list);
}
pure_initcall(init_thread_init);
