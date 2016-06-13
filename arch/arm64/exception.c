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


static const char *exception_names[] = {
	[ESR_EC_PC_ALIGN] = "PC alignment fault",
	[ESR_EC_SP_ALIGN] = "SP alignment fault",
	[ESR_EC_UNKNOWN] = "unknown",
};

// called from vector.S
void __sync_handler(struct eframe *regs, ulong far, u32 esr);

void __sync_handler(struct eframe *regs, ulong far, u32 esr)
{
	uint ec = (esr >> 26);
	uint il = (esr >> 25) & 0x1;
	uint iss = esr & 0x1FFFFFF;
	const char *name;

	switch (ec) {
	case ESR_EC_BRK64:
		return handle_brk(regs, far, esr);
	default:
		break;
	}

	name = exception_names[ec] ? : "???";
	printf("\nException: %s\nfar %016lx esr %08x "
			"(%06b %01b 0x%07x)\n", name, far, esr, ec, il, iss);

	__halt(regs);
}
