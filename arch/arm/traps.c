#include <trap.h>
#include <kernel/bug.h>
#include <exceptions.h>


void __und_handler(const struct eframe *regs);	// used from asm
void __und_handler(const struct eframe *regs)
{
	const unsigned int *pc = (const void *) regs->pc;


	switch (*pc) {
	case TRAP_BUG:
		__bug(regs, NULL, NULL, 0);
		break;

	default:
		und_handler(regs);
	}
}
