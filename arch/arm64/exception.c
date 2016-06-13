#include <exceptions.h>
#include <kernel/bug.h>
#include <arch/esr.h>
#include <halt.h>
#include <trap.h>


static void handle_brk(struct eframe *regs, ulong far, u32 esr)
{
	uint val = esr & 0xFFFF;

	switch (val) {
	case TRAP_BUG:
		__bug(regs, NULL, NULL, 0);
		break;

	default:
		und_handler(regs);
	}
}


// called from vector.S
void __sync_handler(struct eframe *regs, ulong far, u32 esr);

void __sync_handler(struct eframe *regs, ulong far, u32 esr)
{
	uint ec = (esr >> 26);
	uint il = (esr >> 25) & 0x1;
	uint iss = esr & 0x1FFFFFF;

	switch (ec) {
	case ESR_EC_BRK64:
		return handle_brk(regs, far, esr);
	}

	printf("\nException:\nfar %016lx esr %08x "
			"(%06b %01b 0x%07x)\n", far, esr, ec, il, iss);

	__halt(regs);
}
