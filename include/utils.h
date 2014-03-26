#ifndef	_UTILS_H_
#define	_UTILS_H_

#define	ARRAY_SIZE(A)	(sizeof(A)/sizeof(A[0]))

#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - __builtin_offsetof(type,member) );})

#endif
