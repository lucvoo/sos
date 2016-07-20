#include <exceptions.h>
#include <arch/asm-offsets.h>


static void show_nreg(const char *name, ulong reg, const char *sep)
{
	__printf("%3s %016lx%s", name, reg, sep);
}

static void show_xreg(const struct eframe *f, uint i, const char *sep)
{
	__printf("x%02d %016lx%s", i, f->x[i], sep);
}

static void dump_regs(const struct eframe *f)
{
	int i;

	show_nreg("pc", f->pc, " ");
	show_nreg("sp", f->sp, " ");
	show_nreg("lr", f->x[30], " ");
	__printf("psr %08lx\n", f->pstate);

	show_nreg("fp", f->x[29], " ");
	show_xreg(f, 28, " ");
	show_xreg(f, 27, "\n");

	for (i = 26; i > 0; ) {
		show_xreg(f, i--, " ");
		show_xreg(f, i--, " ");
		show_xreg(f, i--, "\n");
	}
}

void __arch_dump_stack(const struct eframe *f)
{
	dump_regs(f);
}
