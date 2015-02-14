#include <arch/io.h>
#include <hw/16550.h>


// Before including this file define:
//	UART_BASE
//	UART_OFF


#define OFF_THR		UART_OFF(UART_THR)
#define OFF_LCR		UART_OFF(UART_LCR)
#define OFF_LSR		UART_OFF(UART_LSR)


static void __diag_putc(unsigned char c)
{
	volatile void *base = (volatile void*) UART_BASE;
	int timeout = 8192;

	if (!ioread8(base + OFF_LCR))
		return;

	while (1) {
		unsigned int status = ioread8(base + OFF_LSR);

		#define	bits	UART_LSR_THRE
		if ((status & bits) == bits)
			break;
		if (--timeout == 0)
			return;
	}

	iowrite8(base + OFF_THR, c);
}
