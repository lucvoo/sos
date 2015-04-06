#include <init.h>
#include <iomap.h>
#include <arch/memory.h>
#include <io.h>
#include <soc/baseaddr.h>
#include <soc/gpio.h>
#include <gpio.h>


static void jz4780_init(void)
{
	printf("JZ4780 booting ...\n");

#ifdef	CONFIG_JTAG_ENABLE
	// Enable JTAG/Disable UART3
	iowrite32(KSEG1_BASE + GPIO_BASE + GPIO_INTC(0),  3 << 30);
	iowrite32(KSEG1_BASE + GPIO_BASE + GPIO_MSKC(0),  3 << 30);
	iowrite32(KSEG1_BASE + GPIO_BASE + GPIO_PAT1C(0), 3 << 30);
	iowrite32(KSEG1_BASE + GPIO_BASE + GPIO_PAT0C(0), 3 << 30);
#endif
}
core_initcall(jz4780_init);


#ifdef CONFIG_ETHERNET
static void jz4780_init_ethernet(void)
{
	struct gpio *eth_int;

	eth_int = gpio_get("gpe", 19);
	if (!eth_int) {
		pr_err("Can't get gpio for ethernet interrupt\n");
	} else {
		gpio_interrupt(eth_int, GPIO_TRIG_EDGE_RISE);
	}
}
board_late_initcall(jz4780_init_ethernet);
#endif
