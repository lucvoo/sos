#ifndef	_KMALLOC_INTERNAL_H_
#define	_KMALLOC_INTERNAL_H_

void *__kmalloc(uint size, uint aflags);
void __kfree(const void *ptr);
uint __kmalloc_size(const void *ptr);

#endif
