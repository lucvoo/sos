#include <kapi/kapi.h>
#include <version.h>
#include <diag.h>
#include <gpio.h>
#include <timerdev.h>


void kapi_start(void)
{
	struct gpio *gpio;
	ulong ref = 0;
	int x = 0;

	gpio = gpio_get("PB", 2);
	if (!gpio) {
		printf("GPIO ????\n");
		return;
	}

	gpio_output(gpio, x);
	printf("OK\n");
	while (1) {
		ulong cur;

		while ((cur = (timerdev_read() & 0xFFFFFFFF)) <= ref)
			;
		gpio_set_value(gpio, x);
	//printf("%08lx -> %08lx\n", ref, cur);
		x = !x;
		ref = cur;
	}
}
