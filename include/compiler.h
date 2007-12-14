#ifndef	_COMPILER_H_
#define	_COMPILER_H_

#define	likely(x)		__builtin_expect(!!(x), 1)
#define	unlikely(x)		__builtin_expect(!!(x), 0)

#define __used			__attribute__((__used__))
#define __section(name)		__attribute__((__section__(name)))
#define __noreturn		__attribute__((noreturn))
#define __uninit		__attribute__((section(".uninit")))

#define barrier() __asm__ __volatile__("": : :"memory")

#define	offsetof(type, member)	__builtin_offsetof(type, member)

#endif
