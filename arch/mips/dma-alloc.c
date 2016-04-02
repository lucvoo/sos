#include <dma-mapping.h>
#include <arch/cacheflush.h>
#include <arch/cachesize.h>
#include <bitops/findbit.h>
#include <arch/types.h>
#include <page-alloc.h>
#include <kmalloc.h>
#include <memory.h>
#include <align.h>
#include <pow2.h>


static inline uint dma_page_order(ulong size)
{
	if (size <= PAGE_SIZE)
		return 0;

	return bitop_log2(size) - PAGE_SHIFT;
}

/**
 * For now only allocate full pages
 * (mostly because we don't have an efficient kmalloc_align())
 */
void *dma_alloc_coherent(ulong size, dma_addr_t *dma_addr, ulong aflags)
{
	struct page *page;
	uint order;
	ulong phys;
	void *norm;	// normal, cached mapping
	void *dma;	// dma, uncached mapping
	ulong p;

	// FIXME: validate the flags (only allow GFP_DMA & GFP_ATOMIC)

	order = dma_page_order(size);

	page = page_alloc(order, GFP_DMA);
	if (!page)
		return NULL;

	phys = page_to_phys(page);
	dma = phys_to_kseg1(phys);
	norm = page_to_virt(page);

	// flush the corresponding cached region
	for (p = 0; p < (1UL << order); p++)
		dcache_flush_page((unsigned long) (norm + (p << PAGE_SHIFT)));

	// return the corresponding uncached region
	// and the physical address as the dma's one
	*dma_addr = phys;
	return dma;
}

void dma_free_coherent(void *cpu_addr, dma_addr_t dma_addr, ulong size)
{
	struct page *page;
	uint order;

	// FIXME: validate cpu & dma addresses match

	// FIXME: do we need to flush something or sync here?

	order = dma_page_order(size);
	page = phys_to_page(dma_addr);
	page_free(page, order);
}
