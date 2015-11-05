#include <diag.h>
#include <init.h>
#include <arch/io.h>
#include <soc/uart.h>

#define	UART_BASE	UART1_BASE


static void imx6_putc(void __iomem *base, unsigned int c)
{
	while ((ioread32(base + UART_UTS) & UTS_TXFULL))
		;

	iowrite32(base + UART_UTXD, c);
}

void _os_diag_write_char(unsigned int c)
{
	void __iomem *base = (void __iomem *) UART_BASE;

	if (c == '\n')
		imx6_putc(base, '\r');
	imx6_putc(base, c);
}

#if 0
static void __init imx6_diag_init(void)
{
	void __iomem *ubase = (void __iomem *) UART_BASE;
	void __iomem *rbase = (void __iomem *) RCC_BASE;
	unsigned int uartclk = HCLK/4 * 1000000;
	unsigned int baud = 115200;
	unsigned int div = (uartclk + baud/2) / baud;
	unsigned int oversampling = 16;
	unsigned int mant = div / oversampling;
	unsigned int frac = div % oversampling;

	imx6_diag_init_gpio();

	// enable the clock
	iosl(rbase + RCC_APB1ENR, RCC_APB1EN_USART3EN);

	// configure the UART
	iowrite32(ubase + USART_BRR, USART_BRR_M(mant) | USART_BRR_F(frac));
	iosl(ubase + USART_CR1, USART_CR1_UE | USART_CR1_TE);
}
pure_initcall(imx6_diag_init);
#endif
