#include <arch/io.h>
#include <soc/pl011.h>


// Before including this file define:
//	PL011_BITS	in <soc/pl011.h>	// 8 or 32
//	PL011_BASE


static inline_always void __diag_putc(u8 c)
{
	void __iomem *base = (void __iomem*) PL011_BASE;

	while (rd_pl011(base + PL011_FR) & PL011_FR_TXFF)
		;

	wr_pl011(base + PL011_DR, c);
#if 0

	while (rd_pl011(base + PL011_FR) & PL011_FR_BUSY)
		;
#endif
}
