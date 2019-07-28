#include <diag.h>
#include <stringify.h>
#include <arch/cp15.h>


static void dump_tbl(const u32 *tbl, uint idx)
{
	int i;

	_os_diag_write_char('T');
	_os_diag_write_char(idx);
	_os_diag_write_char(':');
	_os_diag_write_char('\n');
	for (i = 0; i < 4096; i ++) {
		_os_diag_hexval(i, ':');
		_os_diag_hexval(tbl[i], '\n');
	}
}

void early_tlb_dump(void);
void early_tlb_dump(void)
{
	u32 ttbcr;
	u32 *tbl;

	asm volatile ("mrc " STRINGIFY(TTBCR(%0)): "=r" (ttbcr) :: "memory");
	_os_diag_write_char('T');
	_os_diag_write_char('C');
	_os_diag_write_char(':');
	_os_diag_hexval(ttbcr, '\n');

	asm volatile ("mrc " STRINGIFY(TTBR0(%0)): "=r" (tbl) :: "memory");
	tbl = (u32*) (0xFFFFC000 & ((unsigned long) tbl));
	dump_tbl(tbl, '0');
}
