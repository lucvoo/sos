#include <page.h>
#include <page-alloc.h>
#include <string.h>
#include <memory.h>
#include <init.h>


extern struct page pagemap[];

static void *heap_top;


static unsigned int get_pfn_order(unsigned long pfn)
{
	return __builtin_ctz(pfn | (1 << MAX_ORDER));
}

static void __init pages_add(unsigned long pfn, unsigned long nbr)
{
	while (nbr) {
		unsigned int order = get_pfn_order(pfn);
		unsigned long size = 1 << order;

		if (nbr < size)
			order = get_pfn_order(nbr);

		page_free(pfn_to_page(pfn), order);

		pfn += size;
		nbr -= size;
	}
}


static int __init paging_init(unsigned long nbr)
{
	unsigned long size;
	unsigned long pfn;

	// "reserve" the storage for the pagemap
	size = PAGE_ALIGN(nbr * sizeof(struct page));
	heap_top = ((void *) &pagemap[0]) + size;

	// and initialize it
	memset(pagemap, 0, size);


	// add all the unused pages to the page freelists
	pfn = __phys_to_pfn(virt_to_phys(heap_top));
	pages_add(pfn, nbr - (pfn - PFN_OFFSET));

	return 0;
}

static void __init init_paging(void)
{
	paging_init(CONFIG_PHYS_SIZE >> PAGE_SHIFT);
}
mm_initcall(init_paging);
