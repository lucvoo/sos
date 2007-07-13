#include <thread.h>


struct thread init_thread __attribute__((__section__(".init.task"))) = {
	.cpu_context	= { },
	.entry_point	= 0 + 0,
	.entry_data	= 0,
	.priority	= 0,
	.run_list	= DLIST_INIT(init_thread.run_list),
	.state		= 0,
};
