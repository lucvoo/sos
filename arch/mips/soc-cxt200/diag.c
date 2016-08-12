#include <diag.h>
#include <io.h>
#include <soc/baseaddr.h>


#define	UART_BASE	UART1_BASE
#include <soc/16550.h>
#include <hw/16550-diag-putc.h>


void _os_diag_write_char(uint c)
{
	if (c == '\n')
		__diag_putc('\r');
	__diag_putc(c);
}
