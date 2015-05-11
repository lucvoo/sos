#include <diag.h>
#include <init.h>
#include <arch/io.h>
#include <mach/usart.h>


#define	USART_BASE	USART3_BASE


static void stm32_putc(void __iomem *base, unsigned int c)
{
	while ((ioread32(base + USART_SR) & USART_SR_TXE) == 0)
		;

	iowrite32(base + USART_DR, c);
}

void _os_diag_write_char(unsigned int c)
{
	void __iomem *base = (void __iomem *) USART_BASE;

	if (c == '\n')
		stm32_putc(base, '\r');
	stm32_putc(base, c);
}
