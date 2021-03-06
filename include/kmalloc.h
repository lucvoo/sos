#ifndef	_KMALLOC_H_
#define	_KMALLOC_H_

#include <alloc-flags.h>


void kfree(const void *ptr);
void *kmalloc(unsigned int size, unsigned int flags);
void *kzalloc(unsigned int size, unsigned int flags);
void *krealloc(void *ptr, unsigned int size, unsigned int flags);

#define	knew(type, aflags)	((type *) kmalloc(sizeof(type), aflags))

#endif
