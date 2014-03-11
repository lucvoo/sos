#ifndef	_INIT_H_
#define	_INIT_H_

#include "compiler.h"


#define	__init		__section(".init.text")


#define	__initcall(level,fn) \
	static void (*__initcall_##fn)(void) __used __section(".initcall" level ".text") = fn


#define	pure_initcall(fn)		__initcall("0",fn)
#define	early_initcall(fn)		__initcall("1",fn)
#define	mm_initcall(fn)			__initcall("2",fn)
#define	core_initcall(fn)		__initcall("3",fn)
#define	subsys_initcall(fn)		__initcall("4",fn)
#define	board_initcall(fn)		__initcall("5",fn)
#define	device_initcall(fn)		__initcall("6",fn)
#define	late_initcall(fn)		__initcall("7",fn)

#endif
