#include <thread.h>
#include <sched.h>
#include <stddef.h>


#ifndef CONFIG_THREAD_STACK
#include <page.h>
#include <memory.h>
#include <page-alloc.h>
#include <errno.h>


#define	DEFAULT_STACK_PAGE_ORDER	1

static int thread_stack(struct thread* t, void *stack, unsigned int size)
{
	if (!stack) {
		struct page *p;

		if (!size)
			size = PAGE_SIZE << DEFAULT_STACK_PAGE_ORDER;

		p = page_alloc(DEFAULT_STACK_PAGE_ORDER, GFP_KERN);
		if (!p)
			return -ENOMEM;
		stack = page_to_virt(p);
	}

	t->stack_base = stack;
	t->stack_size = size;
	return 0;
}

#else

static int thread_stack(struct thread* t, void *stack, unsigned int size)
{
	return 0;
}

#endif


int thread_create(struct thread* t, int prio, void (*func)(void*), void* data, void *stack, unsigned int size)
{
	int rc;

	t->priority	= prio;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	rc = thread_stack(t, stack, size);
	if (rc)
		return rc;

	t->run_list.next = NULL;

	thread_load_context(t, func, data);

	return 0;
}
