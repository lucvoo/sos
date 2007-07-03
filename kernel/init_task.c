#include <thread.h>


struct thread init_thread = {
	.cpu_context	= { },
	.stack_base	= 0 + 0,
	.stack_size	= 0,
	.entry_point	= 0 + 0,
	.entry_data	= 0,
	.priority	= 0,
	.run_list	= DLIST_INIT(init_thread.run_list),
	.state		= 0,
};
