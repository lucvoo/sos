#ifndef _ARCH_TEST_FAULT_H_
#define _ARCH_TEST_FAULT_H_

#include <trap.h>

#define	undef_instruction()	__TRAP(TRAP_TEST)

#endif
