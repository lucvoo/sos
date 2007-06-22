#ifndef	_COMPILER_H_
#define	_COMPILER_H_

#define __used			__attribute__((__used__))
#define __section(name)		__attribute__((__section__(name)))

#define barrier() __asm__ __volatile__("": : :"memory")

#endif
