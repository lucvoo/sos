#ifndef	_ARCH_IRQFLAGS_H_
#define	_ARCH_IRQFLAGS_H_

#include "arch/arch.h"

#if CONFIG_ARM_ARCH >= 6
#if !defined(CONFIG_ARMV7M)
#define	__irqflags_save(R)	"mrs	" R ", cpsr\n"
#define	__irqflags_rest(R)	"msr	cpsr_c, " R "\n"
#else
#define	__irqflags_save(R)	"mrs	" R ", primask\n"
#define	__irqflags_rest(R)	"msr	primask, " R "\n"
#endif

static inline void __raw_irq_dis(void)
{
	__asm__ __volatile__("cpsid	i" ::: "memory", "cc");
}

static inline unsigned long __must_check __raw_irq_save(void)
{
	unsigned long flags;

	__asm__ __volatile__(
	__irqflags_save("%0")
"	cpsid	i\n"
	: "=r" (flags)
	:
	: "memory", "cc");

	return flags;
}

static inline void __raw_irq_ena(void)
{
	__asm__ __volatile__("cpsie	i" ::: "memory", "cc");
}

static inline void __raw_irq_rest(unsigned long flags)
{
	__asm__ __volatile__(
	__irqflags_rest("%0")
	:
	: "r" (flags)
	: "memory", "cc");
}

#else
static inline void __raw_irq_dis(void)
{
	unsigned long tmp;

	__asm__ __volatile__(
	"mrs	%0, cpsr \n"
"	orr	%0, %0, %1\n"
"	msr	cpsr_c, %0"
	: "=r" (tmp)
	: "i" (PSR_I)
	: "memory", "cc");
}

static inline unsigned long __must_check __raw_irq_save(void)
{
	unsigned long flags;
	unsigned long tmp;

	__asm__ __volatile__(
	"mrs	%0, cpsr		@ irq save\n"
"	orr	%1, %0, %2\n"
"	msr	cpsr_c, %1"
	: "=r" (flags), "=r" (tmp)
	: "i" (PSR_I)
	: "memory", "cc");

	return flags;
}

static inline void __raw_irq_ena(void)
{
	unsigned long tmp;

	__asm__ __volatile__(
	"mrs	%0, cpsr		@ irq ena\n"
"	bic	%0, %0, %1\n"
"	msr	cpsr_c, %0"
	: "=r" (tmp)
	: "i" (PSR_I)
	: "memory", "cc");
}

static inline void __raw_irq_rest(unsigned long flags)
{
	__asm__ __volatile__(
	"msr	cpsr_c, %0		@ irq rest\n"
	:
	: "r" (flags)
	: "memory", "cc");
}

#endif

#endif
