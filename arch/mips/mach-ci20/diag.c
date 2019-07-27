#include <diag.h>
#include <soc/baseaddr.h>
#include <arch/iomap.h>
#include <init.h>


#define	DIAG_BASE	__ioremap(UART4_BASE)
#include <hw/16550-diag-putc.h>


void _os_diag_write_char(unsigned int c)
{
	if (c == '\n')
		__diag_putc('\r');
	__diag_putc(c);
}

static void __init jz4780_diag_init(void)
{
	/* FIXME */
}
pure_initcall(jz4780_diag_init);
