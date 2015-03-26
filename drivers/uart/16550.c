#include "driver.h"
#include <uart.h>
#include <uapi/errno.h>
#include <io.h>
#include <interrupt.h>
#include <irq.h>
#include <hw/16550.h>
#include <init.h>


struct uart_16550 {
	struct uart	uart;

	void __iomem*	iobase;
	struct irqdesc*	irq;
	struct irqaction	irqaction;
};

#define	to_uart_16550(up)	container_of(up, struct uart_16550, uart)

#include <mach/config-uart-16550.h>
#define	reg_r(p, R)	ioread8(p->iobase + UART_REG(R))
#define	reg_w(p, R, V)	iowrite8(p->iobase + UART_REG(R), V)


static unsigned int uart_16550_rx_chars(struct uart *up, unsigned int lsr)
{
	struct uart_16550 *p = to_uart_16550(up);
	unsigned int max_count = 256;

	do {
		unsigned int flag;
		unsigned int ch;

		if (likely(lsr & UART_LSR_DR))
			ch = reg_r(p, UART_RBR);
		else
			ch = 0;

		flag = CHF_NORMAL;
		uart_inc_stats(up, rx);

#define	UART_LSR_ERRBITS (UART_LSR_FE|UART_LSR_PE|UART_LSR_OE|UART_LSR_BI)
		if (unlikely(lsr & UART_LSR_ERRBITS)) {
			printf("UART %p: errro detected\n", up);
			goto done;
		}

		up->rx_insert(up, up->rx_data, ch, flag);

done:
		lsr = reg_r(p, UART_LSR);
		if (--max_count == 0)
			break;
	} while (lsr & (UART_LSR_DR | UART_LSR_BI));

	return lsr;
}

static void uart_16550_tx_chars(struct uart *up)
{
	// TODO
}

static int uart_16550_isr(struct irqdesc *desc, void *data)
{
	struct uart *up = data;
	struct uart_16550 *p = to_uart_16550(up);
	unsigned int lsr;
	unsigned int iir;
	int rc = IRQ_HANDLED;

	iir = reg_r(p, UART_IIR);
	if (iir & UART_IIR_NOINT)
		return IRQ_NONE;

	lsr = reg_r(p, UART_LSR);
	if (lsr & (UART_LSR_DR|UART_LSR_BI)) {
		lsr = uart_16550_rx_chars(up, lsr);
		if (up->rx_handle)
			rc |= IRQ_CALL_DSR;
	}
	if (lsr & (UART_LSR_THRE))
		uart_16550_tx_chars(up);

	return rc;
}

static int uart_16550_dsr(struct irqdesc *desc, unsigned int count, void *data)
{
	struct uart *up = data;

	up->rx_handle(up, up->rx_data);

	return 0;
}


static void uart_16550_clear_fifos(struct uart_16550 *p)
{
	reg_w(p, UART_FCR, UART_FCR_FIFOE);
	reg_w(p, UART_FCR, UART_FCR_FIFOE|UART_FCR_RXR|UART_FCR_TXR);
	reg_w(p, UART_FCR, 0);
}

static void uart_16550_clear_interrupts(struct uart_16550 *p)
{
	if (reg_r(p, UART_LSR) & UART_LSR_DR)
		reg_r(p, UART_RBR);
	reg_r(p, UART_IIR);
	reg_r(p, UART_MSR);
}

static int uart_16550_start(struct uart *up)
{
	struct uart_16550 *p = to_uart_16550(up);
	int rc = -ENODEV;

	// clear & disable the FIFOs
	uart_16550_clear_fifos(p);

	// clear the interrupt registers
	uart_16550_clear_interrupts(p);

	// catch buggy UARTs
	if (reg_r(p, UART_LSR) == 0xff) {
		printf("ERR: buggy UART %p\n", up);
		goto end;
	}

	// init
	reg_w(p, UART_LCR, UART_LCR_WLEN8);
	reg_w(p, UART_FCR, UART_FCR_FIFOE|UART_FCR_RXR|UART_FCR_TXR|0x10);
	uart_16550_clear_interrupts(p);

	// enable interrupts
	reg_w(p, UART_IER, UART_IER_RLS|UART_IER_RD);
	irq_unmask(p->irq);

	rc = 0;

end:
	return rc;
}

static int uart_16550_stop(struct uart *up)
{
	struct uart_16550 *p = to_uart_16550(up);
	int rc = -EIO;

	// disable interrupts
	irq_mask(p->irq);
	reg_w(p, UART_IER, 0);

	// disable break condition
	reg_w(p, UART_LCR, reg_r(p, UART_LCR) & ~UART_LCR_BRK);

	// disable FIFOs
	uart_16550_clear_fifos(p);

	// read to reset
	if (reg_r(p, UART_LSR) & UART_LSR_DR)
		reg_r(p, UART_RBR);

	return rc;
}

static struct uart_ops uart_16550_ops = {
	.start		= uart_16550_start,
	.stop		= uart_16550_stop,
};


static struct uart_16550 uarts[UART_16550_NBR_PORTS];


#include <irqdesc.h>
static int uart_16550_init_dev(struct uart_16550 *p, const char *name, unsigned long iobase, int irq)
{
	p->iobase = ioremap(iobase, UART_IOSIZE);
	if (!p->iobase)
		goto no_base;

	p->irq = irq_get_desc(UART_IRQ_CTRL, irq);
	if (!p->irq)
		goto no_irq;

	irq_create(&p->irqaction, uart_16550_isr, uart_16550_dsr, &p->uart, 0);
	irq_attach(p->irq, &p->irqaction);

	p->uart.ops = &uart_16550_ops;
	p->uart.name= name;
	return uart_register_dev(&p->uart);

no_irq:
	// FIXME: iounmap(iobase);
no_base:
	return -ENODEV;
}

static void uart_16550_init(void)
{
#define	UART_ENTRY(IDX, IOBASE, IRQ)	uart_16550_init_dev(&uarts[IDX], "uart" #IDX, IOBASE, IRQ);
#include <mach/config-uart-16550.h>
}
device_initcall(uart_16550_init);
