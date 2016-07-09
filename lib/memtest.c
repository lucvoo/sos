#include <memtest.h>


#define	p_cond(cond, F, ...)	do if (cond) printf(F, ##__VA_ARGS__); while (0)
#define	p_verb(FMT, ...)	p_cond(flags & MEMTEST_VERBOSE, FMT, ##__VA_ARGS__)
#define	p_dump(FMT, ...)	p_cond(flags & MEMTEST_DUMP,    FMT, ##__VA_ARGS__)

static int check(ulong addr, ulong expected, int flags)
{
	ulong *p = (ulong *) addr;
	ulong val = *p;
	int err = 0;

	p_dump("%p: %lx\n", p, val = *p);

	if (val != expected) {
		err++;
	}
	return err;
}

static void write_selfaddr(ulong addr)
{
	ulong *p = (ulong *) addr;

	*p = addr;
}

static int check_selfaddr(int flags, ulong addr)
{
	return check(addr, addr, flags);
}

static void read_addr(int flags, ulong addr)
{
	check(addr, -1, flags);
}


void memtest_probe(ulong addr, ulong size, u32 gran, int flags)
{
	ulong i;

	printf("read ... ");
	p_dump("\n");
	for (i = 0; i < size; i += gran) {
		read_addr(flags, addr + i);
	}
	printf("done\n");
}

int memtest_simple(ulong addr, ulong size, u32 gran, int flags)
{
	int err = 0;
	u32 i;

	printf("write ... ");
	for (i = 0; i < size; i += gran) {
		write_selfaddr(addr + i);
		write_selfaddr(addr + i + gran - sizeof(ulong));
	}
	printf("done\n");

	printf("read back ... ");
	p_dump("\n");
	for (i = 0; i < size; i += gran) {
		err += check_selfaddr(flags, addr + i);
		err += check_selfaddr(flags, addr + i + gran - sizeof(ulong));
	}
	printf("done: %s\n", err == 0 ? "OK" : "FAILED!");

	return !!err;
}
