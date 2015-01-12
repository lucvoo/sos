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

	//printf("%s(): free %lu pages\n", __func__, n);
	return n;
}

static int test_alloc_free_all(void)
{
	unsigned long n = 0;
	unsigned int i = 0;
	unsigned long f;

	while (1) {
		unsigned int order;
		struct page *p;

		order = sprng() % 3;

		p = page_alloc(order, 0);
		if (p) {
			n += 1 << order;
			pages[i++] = ((struct pages) { p, order, });
		}

		//printf("page_alloc(%2u) => page:%p, pfn:%lx (tot:%lu)\n", order, p, page_to_pfn(p), n);

		if (!p && order == 0)
			break;
	}

	//printf("DEBUG: no memory left! (nbr alloc = %u)\n", i);

	f = free_allocated_pages(i);
	if (f != n) {
		printf("ERROR! %s(): f = %lu but n was %lu\n", __func__, f, n);
		return 1;
	}

	return 0;
}

#define RESULT(X)	if (X) ko++; else ok++

static void fun(void* data)
{
	unsigned int ok = 0;
	unsigned int ko = 0;
	int i;

	for (i = 0; i < 64; i++) {
		RESULT(test_alloc_free_all());
	}

	printf("TEST page-alloc: fail = %u, succeed = %i\n", ko, ok);
	thread_sleep();
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&t, 2, fun, NULL);
	thread_start(&t);
}
