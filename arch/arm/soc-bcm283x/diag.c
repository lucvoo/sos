#include <diag.h>
#include <soc/pl011.h>
#include <hw/pl011-diag-putc.h>


void _os_diag_write_char(uint c)
{
	if (c == '\n')
		__diag_putc('\r');
	__diag_putc(c);
}
