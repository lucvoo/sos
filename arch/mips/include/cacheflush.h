#ifndef	_ARCH_CACHEFLUSH_H_
#define	_ARCH_CACHEFLUSH_H_


// Write-back
void dcache_clean_range(ulong start, ulong size);
void dcache_clean_page(ulong page);

// Write-back & invalidate
void dcache_flush_range(ulong start, ulong size);
void dcache_flush_page(ulong page);
void dcache_flush_all(void);

// Invalidate
void dcache_inval_range(ulong start, ulong size);
void dcache_inval_page(ulong page);
void dcache_inval_all(void);

#endif
