#ifndef	_SLAB_H_
#define	_SLAB_H_

struct kmem_cache;

void *kmem_alloc(struct kmem_cache *cache);
void kmem_free(struct kmem_cache *cache, const void *obj);

struct kmem_cache *kmem_cache_create(const char *name,
				     unsigned int size,
				     unsigned int align,
				     unsigned long flags);

#endif
