#ifndef	_ARCH_IRQFLAGS_H_
#define	_ARCH_IRQFLAGS_H_


static inline void __raw_irq_dis(void)
{
	asm volatile(
	"	msr	daifset, #2"
	::: "memory");
}

static inline ulong __must_check __raw_irq_save(void)
{
	ulong flags;

	asm volatile(
	"	mrs	%0, daif\n"
	"	msr	daifset, #2"
	: "=r" (flags)
	:
	: "memory");

	return flags;
}

static inline void __raw_irq_ena(void)
{
	asm volatile(
	"	msr	daifclr, #2"
	::: "memory");
}

static inline void __raw_irq_rest(ulong flags)
{
	asm volatile(
	"	msr	daif, %0"
	:
	: "r" (flags)
	: "memory");
}

#endif
