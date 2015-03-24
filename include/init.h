#ifndef	_INIT_H_
#define	_INIT_H_


#define	__init		__section(".init.text")
#define	__initdata	__section(".init.data")


struct initcall {
	void (*fun)(void);
};

#define	__initcall(level,fn) \
	static struct initcall __initcall_##fn __used __section(".initcall" level ".init") = { \
		.fun = fn, \
	}

#define	pure_initcall(fn)		__initcall("0",fn)
#define	early_initcall(fn)		__initcall("1",fn)
#define	mm_initcall(fn)			__initcall("2",fn)
#define	core_initcall(fn)		__initcall("3",fn)
#define	subsys_initcall(fn)		__initcall("4",fn)
#define	board_probe_initcall(fn)	__initcall("5a",fn)
#define	board_setup_initcall(fn)	__initcall("5b",fn)
#define	board_irq_initcall(fn)		__initcall("5c",fn)
#define	board_core_initcall(fn)		__initcall("5d",fn)
#define	board_initcall(fn)		__initcall("5e",fn)
#define	board_late_initcall(fn)		__initcall("5f",fn)
#define	driver_initcall(fn)		__initcall("6",fn)
#define	device_initcall(fn)		__initcall("7",fn)
#define	late_initcall(fn)		__initcall("8",fn)


#ifdef	CONFIG_REMOVE_INIT_SECTIONS
void __free_initmem(void);
#else
#define	__free_initmem()	do ; while (0)
#endif

#endif
