#include <exceptions.h>


// Called from assembler: arch/arm/vector-XX.S
void pabt_handler(const struct eframe *regs, unsigned long far, unsigned long fsr);
void dabt_handler(const struct eframe *regs, unsigned long far, unsigned long fsr);


void pabt_handler(const struct eframe *regs, unsigned long far, unsigned long fsr)
{
	printf("PABT: FAR=%08lx, FSR=%08lx\n", far, fsr);
	dump_stack(regs, 0);
	while (1)
		;
}

void dabt_handler(const struct eframe *regs, unsigned long far, unsigned long fsr)
{
	printf("DABT: FAR=%08lx, FSR=%08lx\n", far, fsr);
	dump_stack(regs, 0);
	while (1)
		;
}
