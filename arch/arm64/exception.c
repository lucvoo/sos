#include <exceptions.h>
#include <halt.h>


// called from vector.S
void __sync_handler(struct eframe *regs, ulong far, u32 esr);

void __sync_handler(struct eframe *regs, ulong far, u32 esr)
{
	uint ec = (esr >> 26);
	uint il = (esr >> 25) & 0x1;
	uint iss = esr & 0x1FFFFFF;


	printf("\nException:\nfar %016lx esr %08x "
			"(%06b %01b 0x%07x)\n", far, esr, ec, il, iss);

	__halt(regs);
}
