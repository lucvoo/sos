objs-y	+= 	dump-stack.o
objs-y	+= 	init.o
objs-y	+= 	thread.o
objs-y	+= 	traps.o

objs-$(CONFIG_CCI400)	+= cci-400.o
objs-$(CONFIG_GENERIC_TIMER)	+= generic_timer.o


objs-$(CONFIG_ARMVA)	+= startup.o
objs-$(CONFIG_ARMVA)	+= switch_to.o
objs-$(CONFIG_ARMV4)	+= irq-v4.o
objs-$(CONFIG_ARMV4)	+= undef-v4.o
objs-$(CONFIG_ARMV6)	+= cache-v6.o
objs-$(CONFIG_ARMV6)	+= vector-v6.o
objs-$(CONFIG_ARMV7A)	+= cache-v7.o
objs-$(CONFIG_ARMV7A)	+= perf-v7.o
objs-$(CONFIG_ARMV7A)	+= vector-v6.o
objs-$(CONFIG_ARMV7M)	+= entry-v7m.o
objs-$(CONFIG_ARMV7M)	+= switch-v7m.o


deps-y	:= \
	target.lds	\


subdirs-y			+= lib/
subdirs-y			+= mm/
subdirs-y			+= debug/
subdirs-$(CONFIG_SOC_AM335X)	+= soc-am335x/
subdirs-$(CONFIG_SOC_BCM283X)	+= soc-bcm283x/
subdirs-$(CONFIG_SOC_IMX6)	+= soc-imx6/
subdirs-$(CONFIG_SOC_STM32)	+= soc-stm32/
subdirs-$(CONFIG_SOC_SUNXI)	+= soc-sunxi/

subdirs-$(CONFIG_MACH_BEAGLEBONE)	+= mach-beaglebone/
subdirs-$(CONFIG_MACH_CUBIEBOARD4)	+= mach-cubieboard4/
subdirs-$(CONFIG_MACH_ORANGEPI_ZERO)	+= mach-orangepi-zero/
subdirs-$(CONFIG_MACH_STM32)	+= mach-stm32/
subdirs-$(CONFIG_MACH_UDOO_NEO)	+= mach-udoo-neo/
