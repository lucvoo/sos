#ifndef	_ARCH_IRQFLAGS_H_
#define	_ARCH_IRQFLAGS_H_


static inline void __raw_irq_dis(void)
{
	asm volatile(
	"di				\n\t"
	"ehb				\n\t"
	:
	:
	: "memory");
}

static inline unsigned long __must_check __raw_irq_save(void)
{
	unsigned long flags;

	asm volatile(
	"di	%0			\n\t"
	"ehb				\n\t"
	: "=r" (flags)
	:
	: "memory");

	return flags;
}

static inline void __raw_irq_ena(void)
{
	asm volatile(
	"ei				\n\t"
	"ehb				\n\t"
	:
	:
	: "memory");
}

static inline void __raw_irq_rest(unsigned long flags)
{
	asm volatile(
	"mtc0	%0, $12			\n\t"
	"ehb				\n\t"
	:
	: "r" (flags)
	: "memory");
}

#endif
