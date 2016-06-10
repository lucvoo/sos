#include <exceptions.h>


// called from vector.S
void __sync_handler(struct eframe *regs, ulong far, u32 esr);

void __sync_handler(struct eframe *regs, ulong far, u32 esr)
{
	// TODO
}
