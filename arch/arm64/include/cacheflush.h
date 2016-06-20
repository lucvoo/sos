#ifndef	_ARCH_CACHEFLUSH_H_
#define	_ARCH_CACHEFLUSH_H_


// Write-back
void dcache_clean_range(void *start, ulong size);
void dcache_clean_page(void *page);

// Write-back & invalidate
void dcache_flush_range(void *start, ulong size);
void dcache_flush_page(void *page);
void dcache_flush_all(void);

// Invalidate
void dcache_inval_range(void *start, ulong size);
void dcache_inval_page(void *page);
void dcache_inval_all(void);


#ifdef	CONFIG_DMA_COHERENT
#define	dma_cache_inval(A, S)	do { } while (0)
#define	dma_cache_clean(A, S)	do { } while (0)
#define	dma_cache_flush(A, S)	do { } while (0)
#else
void dma_cache_inval(void *start, ulong size);
void dma_cache_clean(void *start, ulong size);
void dma_cache_flush(void *start, ulong size);
#endif

#endif
