#include <diag.h>
#include <init.h>
#include <arch/io.h>


#define	DIAG_BASE	((void __iomem *) 0x44e09000)
#define	DIAG_THR	(DIAG_BASE + 0x00)
#define	DIAG_LSR	(DIAG_BASE + 0x14)

#define	DIAG_LSR_THRE	0x20


static void beaglebone_diag_putc(unsigned char c)
{
	while ((ioread8(DIAG_LSR) & DIAG_LSR_THRE) == 0)
		;
	iowrite8(DIAG_THR, c);
}

void _os_diag_write_char(unsigned int c)
{
	if (c == '\n')
		beaglebone_diag_putc('\r');
	beaglebone_diag_putc(c);
}

static void __init beaglebone_diag_init(void)
{
	/* FIXME */
}
pure_initcall(beaglebone_diag_init);
