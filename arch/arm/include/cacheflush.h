#ifndef	_ARCH_CACHEFLUSH_H_
#define	_ARCH_CACHEFLUSH_H_


void dcache_clean_range(void *addr, uint size);
void dcache_inval_range(void *addr, uint size);
void dcache_flush_range(void *addr, uint size);

#endif
