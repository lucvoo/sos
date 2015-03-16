#include <exceptions.h>


void __und_handler(const struct eframe *regs);	// used from asm
void __und_handler(const struct eframe *regs)
{
	const unsigned int *pc = (const void *) regs->pc;


	switch (*pc) {
	default:
		und_handler(regs);
	}
}
