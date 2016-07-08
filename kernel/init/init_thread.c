#include <thread.h>
#include <init.h>


struct thread init_thread __uninit;

#ifndef CONFIG_THREAD_STACK
#include <init.h>
#include <page.h>

static unsigned char init_stack[PAGE_SIZE];

static void init_thread_init(void)
{
	init_thread.stack_base = init_stack;
	init_thread.stack_size = sizeof(init_stack);

	set_current_thread(&init_thread);
}
pure_initcall(init_thread_init);
#endif
