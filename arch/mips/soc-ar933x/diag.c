#include <diag.h>
#include <io.h>
#include <arch/iomap.h>
#include <soc/baseaddr.h>
#include <soc/uart.h>


static void __diag_putc(unsigned char c)
{
	void __iomem *iobase = __ioremap(UART_BASE);

	while ((ioread32(iobase + UART_DATA) & UART_DATA_TX) == 0)
		;

	iowrite32(iobase + UART_DATA, c | UART_DATA_TX);
}

void _os_diag_write_char(unsigned int c)
{
	if (c == '\n')
		__diag_putc('\r');
	__diag_putc(c);
}
