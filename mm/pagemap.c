#include <page.h>
#include <string.h>
#include <init.h>


extern struct page pagemap[];

static void *heap_top;

static int __init paging_init(unsigned long nbr)
{
	unsigned long size;

	// "reserve" the storage for the pagemap
	size = PAGE_ALIGN(nbr * sizeof(struct page));
	heap_top = ((void *) &pagemap[0]) + size;

	// and initialize it
	memset(pagemap, 0, size);

	return 0;
}

static void __init init_paging(void)
{
	paging_init(CONFIG_PHYS_SIZE >> PAGE_SHIFT);
}
mm_initcall(init_paging);
