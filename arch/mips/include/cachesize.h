#ifndef	_ARCH_CACHESIZE_H_
#define	_ARCH_CACHESIZE_H_

#include <soc/cache.h>

#define	ICACHE_SIZE	(CACHE_LI_SETS * CACHE_LI_WAYS * CACHE_LI_LINESIZE)
#define	DCACHE_SIZE	(CACHE_LD_SETS * CACHE_LD_WAYS * CACHE_LD_LINESIZE)
#define	DCACHE_LINESIZE	 CACHE_LD_LINESIZE

#endif
