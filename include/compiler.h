#ifndef	_COMPILER_H_
#define	_COMPILER_H_

#define	likely(x)		__builtin_expect(!!(x), 1)
#define	unlikely(x)		__builtin_expect(!!(x), 0)

#define __used			__attribute__((__used__))
#define __unused		__attribute__((__unused__))
#define __section(name)		__attribute__((__section__(name)))
#define __noreturn		__attribute__((noreturn))
#define __uninit		__attribute__((section(".uninit")))

#define inline_always		inline __attribute__ ((always_inline))
#define __noinline		__attribute__ ((noinline))

#define	__must_check		__attribute__((warn_unused_result))

#define	offsetof(type, member)	__builtin_offsetof(type, member)

#define	__aligned(N)		__attribute__((aligned(N)))
#define	__packed		__attribute__((__packed__))


#define	__aligned_stack		__aligned(CONFIG_STACK_ALIGNMENT)
#define	__stack			__aligned_stack __section(".uninit.stack")

#endif
