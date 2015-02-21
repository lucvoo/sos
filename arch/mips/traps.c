#include <exceptions.h>


static const char *exception_names[] = {
	[0x00] = "INT",		// Interrupt
	[0x01] = "TLBM",	// TLB modification exception
	[0x02] = "TLBL",	// TLB exception (load or instruction fetch)
	[0x03] = "TLBS",	// TLB exception (store)
	[0x04] = "AdEL",	// Address error exception (load or instruction fetch)
	[0x05] = "AdES",	// Address error exception (store)
	[0x06] = "IBE",		// Bus error exception (instruction fetch)
	[0x07] = "DBE",		// Bus error exception (data reference: load or store)
	[0x08] = "SYS",		// Syscall exception
	[0x09] = "BREAK",	// Breakpoint exception
	[0x0a] = "RI",		// Reserved instruction exception
	[0x0b] = "COPRO",	// Coprocessor Unusable exception
	[0x0c] = "OVER",	// Arithmetic Overflow exception
	[0x0d] = "TRAP",	// Trap exception
	[0x0e] = "MSAFP",	// MSA Floating Point exception
	[0x0f] = "FPE",		// Floating point exception
	[0x10] = "?10?",	// ?
	[0x11] = "?11?",	// ?
	[0x12] = "C2E",		// Reserved for precise Coprocessor 2 exceptions
	[0x13] = "TLBRI",	// TLB Read Inhibit exception
	[0x14] = "TLBXI",	// TLB Execution Inhibit exception
	[0x15] = "MSAD",	// MSA Disable exception
	[0x16] = "MDMX",	// MDMX Unusable Exception.
	[0x17] = "WATCH",	// Reference to WatchHi/WatchLo address
	[0x18] = "MCHK",	// Machine check
	[0x19] = "MT",		// MT exception
	[0x1a] = "DSPD",	// DSP Disable exception
	[0x1b] = "GUEST",	// Virtualized Guest exception
	[0x1c] = "?1c?",	// ?
	[0x1d] = "?1d?",	// ?
	[0x1e] = "CERR",	// Cache error
	[0x1f] = "?1f?",	// ?
};

extern void __handle_exceptions(struct eframe *regs, unsigned long cause, unsigned long status);


void __handle_exceptions(struct eframe *regs, unsigned long cause, unsigned long status)
{
	unsigned int code = (cause & CAUSE_CODE_MSK) >> CAUSE_CODE_OFF;
	const char *name = exception_names[code];

	printf("EXC: cause = %08X (% 5s), status = %08X, regs @ %p\n", cause, name, status, regs);

	dump_stack(regs, 0);
}


extern void __handle_undef(struct eframe *regs);
void __handle_undef(struct eframe *regs)
{
	printf("\nERROR: undefined instruction @0x%08lx: %08X\n", regs->epc, *(unsigned long *)regs->epc);
	dump_stack(regs, 0);

	// FIXME: how to handle this?
	//	for the moment, just skip the offending instruction
	regs->epc += 4;
}
