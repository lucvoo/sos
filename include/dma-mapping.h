#ifndef	_DMA_MAPPING_H_
#define	_DMA_MAPPING_H_

// TODO: the DMA operations can be device specific

// Note: for now let DMA/bus addresses = 32bit physical addresses

ulong dma_rbuff_map(const void *vaddr, uint size);
ulong dma_wbuff_map(void *vaddr, uint size);

void dma_rbuff_get(ulong baddr, uint size);
void dma_rbuff_put(ulong baddr, uint size);
void dma_wbuff_get(ulong baddr, uint size);
void dma_wbuff_put(ulong baddr, uint size);


#include <types/dma_addr_t.h>
#include <alloc-flags.h>
void *dma_alloc_coherent(ulong size, dma_addr_t *addr, ulong aflags);
void dma_free_coherent(void *cpu_addr, dma_addr_t dma_addr, ulong size);

#endif
