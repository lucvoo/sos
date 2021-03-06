#include <debug.h>
#include <arch/irqflags.h>
#include <stringify.h>


#define	reg(NAME)	({ ulong r; asm("mrs %x0, " NAME : "=r" (r)); r; })
#define	dump(NAME, VAL)	printf("%18s:\t%016lx\n", NAME, VAL)
#define	dump_reg_(NAME)	dump(NAME, reg(NAME))
#define	dump_reg(NAME)		dump_reg_(#NAME)
#define	dump_elx(NAME, L)	dump_reg_(#NAME "_el" #L)
#define	dump_el0(NAME)		dump_elx(NAME, 0)
#define	dump_el1(NAME)		dump_elx(NAME, 1)
#define	dump_el23(NAME) do {						\
				if (el == 3)				\
					dump_elx(NAME, 3);		\
				if (el >= 2)				\
					dump_elx(NAME, 2);		\
			} while (0)


void dump_system_regs(void)
{
	uint el = (reg("currentel") >> 2) & 0x3;

	printf("\n");
	dump_el1(midr);
	dump_el1(mpidr);
	dump_el1(revidr);
	//dump_el1(l2ctlr);

	printf("\n");
	dump_el1(id_aa64pfr0);
	dump_el1(id_aa64isar0);
	dump_el1(id_aa64mmfr0);

	printf("\n");
	dump_el1(ccsidr);
	dump_el1(clidr);
	dump_el1(csselr);
	dump_el0(ctr);
	dump_el0(dczid);

	printf("\n");
	dump_reg(currentel);
	dump_reg(spsel);
	dump_reg(daif);
	dump_reg(nzcv);
	dump_el1(contextidr);
	dump_el1(sctlr);
	dump_el23(sctlr);
	dump_el1(vbar);
	dump_el1(isr);
	dump_el1(tpidr);

	printf("\n");
	dump_el1(tcr);
	dump_el1(ttbr0);
	dump_el23(ttbr0);
	dump_el1(ttbr1);
	dump_el1(mair);
	dump_el23(mair);

	printf("\n");
	if (el >= 2) {
		dump_elx(hcr, 2);
	}
	if (el == 3) {
		dump_elx(scr, 3);
		dump_elx(cptr, 3);
		dump_el23(rvbar);
		//dump_el1(cbar);
	}

	printf("\n");
	dump_el0(cntfrq);

	printf("\n");
	dump_el23(spsr);
	dump_el23(elr);
	dump_el23(vbar);
}
