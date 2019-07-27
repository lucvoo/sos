#include <arch/io.h>
#include <soc/16550.h>


// Before including this file define:
//	UART_STEP	in <soc/16550.h>
//	UART_BITS	in <soc/16550.h>
//	UART_BASE


static inline_always void __diag_putc(unsigned char c)
{
	void __iomem *base = DIAG_BASE;
	int timeout = 8192;

	if (!rd_16550(base, UART_LCR))
		return;

	while (1) {
		unsigned int status = rd_16550(base, UART_LSR);

		#define	bits	UART_LSR_THRE
		if ((status & bits) == bits)
			break;
		if (--timeout == 0)
			return;
	}

	wr_16550(base, UART_THR, c);
}
