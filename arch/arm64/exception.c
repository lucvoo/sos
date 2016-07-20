#include <exceptions.h>
#include <kernel/bug.h>
#include <arch/esr.h>
#include <halt.h>
#include <trap.h>


static void handle_dabort(struct eframe *regs, ulong far, u32 esr)
{
	uint dfsc = esr & 0xFFFF;
	uint level = dfsc & 0x3;
	const char *dlevel = NULL;
	const char *desc = NULL;

#if 0
	if ((esr & ESR_ISS_DABORT_ISV) == 0) {
		desc = "instruction syndrome invalid";
		goto out;
	}
#endif

	switch (dfsc) {
	case 0b000000 ... 0b000011:
		dlevel = "address size fault";
		break;
	case 0b000100 ... 0b000111:
		dlevel = "translation fault";
		break;
	case 0b001000 ... 0b001011:
		dlevel = "access fault";
		break;
	case 0b001100 ... 0b001111:
		dlevel = "permission fault";
		break;
	case 0b010100 ... 0b010111:
		dlevel = "exernal abort";
		break;
	case 0b011100 ... 0b011111:
		dlevel = "parity/ECC fault";
		break;
	case 0b010000:
		desc = "external abort";
		break;
	case 0b011000:
		desc = "parity/ECC error";
		break;
	case 0b100001:
		desc = "alignment";
		break;
	case 0b110000:
		desc = "TLB conflict";
		break;
	case 0b110100:
		desc = "lockdown fault";
		break;
	case 0b110101:
		desc = "exclusive access fault";
		break;
	case 0b111101:
		desc = "section domain fault";
		break;
	case 0b111110:
		desc = "page domain fault";
		break;
	default:
		desc = "unknown fault";
		goto out;
	}

out:
	if (dlevel) {
		printf("\nData Abort: %s level %d\n", dlevel, level);
	} else
		printf("\nData Abort: %s\n", desc);
	printf("far %016lx, esr %08llx\n", far, esr);
	__halt(regs);
}

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
	[ESR_EC_IABORT_EL0] = "instruction abort from lower level",
	[ESR_EC_DABORT_EL0] = "data abort from lower level",
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
	case ESR_EC_DABORT_EL1:
		return handle_dabort(regs, far, esr);
	default:
		break;
	}

	name = exception_names[ec] ? : "???";
	printf("\nException: %s\nfar %016lx esr %08x "
			"(%06b %01b 0x%07x)\n", name, far, esr, ec, il, iss);

	__halt(regs);
}
