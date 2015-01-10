#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include "tests-utils.h"
#include <page.h>
#include <page-alloc.h>
#include <memory.h>


static struct thread t __uninit;

#define	MAX_ALLOCS	65536

static struct pages {
	struct page *p;
	unsigned int o;
} pages[MAX_ALLOCS];


static unsigned long free_allocated_pages(unsigned int i)
{
	unsigned long n = 0;

	while (i--) {
		struct pages *p = &pages[i];
		unsigned int order = p->o;

		page_free(p->p, order);
		n += 1 << order;
	}

	printf("%s(): free %lu pages\n", __func__, n);
	return n;
}

static void fun(void* data)
{
	unsigned long n = 0;
	unsigned int i = 0;

	while (1) {
		unsigned int order;
		struct page *p;

		do
			order = sprng() & 0x3;
		while (order > MAX_ORDER);

		p = page_alloc(order, 0);

		if (p) {
			n += 1 << order;
			pages[i++] = ((struct pages) { p, order, });
		}

		//printf("page_alloc(%2u) => page:%p, pfn:%lx (tot:%lu)\n", order, p, page_to_pfn(p), n);

		if (!p && order == 0) {
			unsigned long f;

			printf("no memory left! (nbr alloc = %u)\n", i);

			f = free_allocated_pages(i);
			if (f != n) {
				printf("ERROR! f = %lu but n was %lu\n", f, n);
				break;
			}

			n = 0;
			i = 0;
		}
	}

	thread_sleep();
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&t, 2, fun, NULL);
	thread_start(&t);
}
