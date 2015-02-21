#include <exceptions.h>


extern void __handle_exceptions(struct eframe *regs, unsigned long cause, unsigned long status);


void __handle_exceptions(struct eframe *regs, unsigned long cause, unsigned long status)
{
	printf("EXC: cause = %08X, status = %08X, regs @ %p\n", cause, status, regs);

	dump_stack(regs, 0);
}
