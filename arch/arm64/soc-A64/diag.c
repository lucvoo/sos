#include <diag.h>
#include <io.h>


#define	UART_BASE	UARTX_BASE(0)

#include <soc/16550.h>
#include <hw/16550-diag-putc.h>

void _os_diag_write_char(unsigned int c)
{
	if (c == '\n')
		__diag_putc('\r');
	__diag_putc(c);
}
