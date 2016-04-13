#include <halt.h>
#include <exceptions.h>


void __noreturn __halt(const struct eframe *regs)
{
	if (regs)
		dump_stack(regs, 0);

	while (1)
		;
}
