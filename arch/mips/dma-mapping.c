#include <dma-mapping.h>
#include <arch/cacheflush.h>
#include <arch/memory.h>


// FIXME: for now let DMA/bus addresses = 32bit physical addresses
#define virt_to_dma(X)	virt_to_phys(X)
#define dma_to_virt(X)	phys_to_virt(X)
#define dma_to_page(X)	phys_to_page(X)


static inline void __dma_sync_virt(ulong addr, uint size, int write)
{
	if (write)
		dma_cache_clean(addr, size);
	else
		dma_cache_inval(addr, size);
}

static inline void __dma_sync(ulong baddr, uint size, int write)
{
#ifdef	CONFIG_DMA_COHERENT
#ifdef	CONFIG_HIGHMEM
	struct page *page = dma_to_page(baddr);
	uint off = baddr & ~PAGE_MASK;

	dma_sync_page(page, off, size, write);
#else
	dma_sync_virt((ulong)dma_to_virt(baddr), size, write);
#endif
#endif
}


// TODO: the DMA operations can be device specific

ulong dma_rbuff_map(const void *vaddr, uint size)
{
	ulong baddr = virt_to_dma(vaddr);

	return baddr;
}

ulong dma_wbuff_map(void *vaddr, uint size)
{
	ulong baddr = virt_to_dma(vaddr);

	__dma_sync(baddr, size, 1);
	return baddr;
}


void dma_rbuff_put(ulong baddr, uint size)
{
	__dma_sync(baddr, size, 0);
}

void dma_rbuff_get(ulong baddr, uint size)
{
#ifdef	CONFIG_SPECULATIVE_ACCESS
#error
#endif
}

void dma_wbuff_put(ulong baddr, uint size)
{
	__dma_sync(baddr, size, 1);
}

void dma_wbuff_get(ulong baddr, uint size)
{
#ifdef	CONFIG_SPECULATIVE_ACCESS
#error
#endif
}
