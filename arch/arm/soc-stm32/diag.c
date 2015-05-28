#include <diag.h>
#include <init.h>
#include <arch/io.h>
#include <soc/baseaddr.h>
#include <soc/usart.h>
#include <soc/gpio.h>
#include <soc/rcc.h>


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


static void iosl(void __iomem *addr, unsigned int val)
{
	iowrite32(addr, ioread32(addr) | val);
}

static void stm32_diag_init_gpio(void)
{
	// PB10 & PB11 => AF 7
#define	PORT 1				// GPIO B

	void __iomem *base = (void __iomem*) GPIO_BASE(PORT);
	void __iomem *rbase = (void __iomem *) RCC_BASE;

	// enable the clock
	iosl(rbase + RCC_AHB1ENR, RCC_AHB1EN_GPIOEN(PORT));

	iosl(base + GPIO_MODER, GPIO_MODE_AF(10)|GPIO_MODE_AF(11));
	iosl(base + GPIO_OTYPER, GPIO_OTYPE_PP(10)|GPIO_OTYPE_PP(11));
	iosl(base + GPIO_SPEEDR, GPIO_SPEED_FAST(10)|GPIO_SPEED_FAST(11));
	iosl(base + GPIO_PUPDR, GPIO_PUPD_UP(10)|GPIO_PUPD_UP(11));
	iosl(base + GPIO_AFR(10), GPIO_AF(10, 7)|GPIO_AF(11, 7));
}

static void __init stm32_diag_init(void)
{
	void __iomem *ubase = (void __iomem *) USART_BASE;
	void __iomem *rbase = (void __iomem *) RCC_BASE;
	unsigned int uartclk = 42000000;	// FIXME
	unsigned int baud = 115200;
	unsigned int div = (uartclk + baud/2) / baud;
	unsigned int oversampling = 16;
	unsigned int mant = div / oversampling;
	unsigned int frac = div % oversampling;

	stm32_diag_init_gpio();

	// enable the clock
	iosl(rbase + RCC_APB1ENR, RCC_APB1EN_USART3EN);

	// configure the UART
	iowrite32(ubase + USART_BRR, USART_BRR_M(mant) | USART_BRR_F(frac));
	iosl(ubase + USART_CR1, USART_CR1_UE | USART_CR1_TE);
}
pure_initcall(stm32_diag_init);
