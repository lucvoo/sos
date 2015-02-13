#ifndef _ARCH_TEST_FAULT_H_
#define _ARCH_TEST_FAULT_H_


#define	undef_instruction()	__asm__ __volatile(".word 0x7fffffff")

#endif
