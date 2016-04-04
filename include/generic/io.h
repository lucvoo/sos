#ifndef	_GENERIC_IO_H_
#define	_GENERIC_IO_H_

#include <iomem.h>

#define iowrite8(a, v)	(__chk_io_ptr(a), *(volatile       u8  __force *)(a) = (v))
#define iowrite16(a, v)	(__chk_io_ptr(a), *(volatile       u16 __force *)(a) = (v))
#define iowrite32(a, v)	(__chk_io_ptr(a), *(volatile       u32 __force *)(a) = (v))

#define ioread8(a)	(__chk_io_ptr(a), *(volatile const u8  __force *)(a))
#define ioread16(a)	(__chk_io_ptr(a), *(volatile const u16 __force *)(a))
#define ioread32(a)	(__chk_io_ptr(a), *(volatile const u32 __force *)(a))

#endif
