#include <thread.h>
#include <sched.h>
#include <stddef.h>
#include <errno.h>


#ifndef CONFIG_THREAD_STACK
#include <page.h>
#include <memory.h>
#include <page-alloc.h>
#include <bug.h>


#define	DEFAULT_STACK_PAGE_ORDER	1

static void* thread_stack_top(struct thread* t, void *stack, unsigned int size)
{
	if (!stack) {
		struct page *p;

		if (!size)
			size = PAGE_SIZE << DEFAULT_STACK_PAGE_ORDER;

		p = page_alloc(DEFAULT_STACK_PAGE_ORDER, GFP_KERN);
		if (!p)
			return NULL;	// FIXME: need ERR_PTR(ENOMEM)
		stack = page_to_virt(p);
	} else {
		ulong base = (ulong) stack;

		BUG_ON((base | size) % CONFIG_STACK_ALIGNMENT);
	}

	return stack + size;
}

#else

static void* thread_stack_top(struct thread* t, void *stack, unsigned int size)
{
	return (stack = t) + THREAD_SIZE;
}

#endif


int thread_create(struct thread* t, int prio, void (*func)(void*), void* data, void *stack, unsigned int size)
{
	void *stack_top;

	t->priority	= prio;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	stack_top = thread_stack_top(t, stack, size);
	if (!stack_top)
		return -ENOMEM;		// FIXME: need ERR_PTR

	t->run_list.next = NULL;

	thread_load_context(t, func, data, stack_top);

	return 0;
}
