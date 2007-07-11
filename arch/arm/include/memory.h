#ifndef	_ARCH_MEMORY_H_
#define	_ARCH_MEMORY_H_

#ifndef	__ASSEMBLY__
#define	UL(x)	(x##UL)
#else
#define	UL(x)	(x)
#endif


#define	VIRT_ADDR	UL(CONFIG_VIRT_ADDR)
#define	TEXT_OFFSET	UL(CONFIG_TEXT_OFFSET)

#endif
