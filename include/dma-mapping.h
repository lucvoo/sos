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

#endif
