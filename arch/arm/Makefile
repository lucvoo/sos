objs-y	+= 	context.o
objs-y	+= 	dump-stack.o
objs-y	+= 	init.o
objs-y	+= 	traps.o


objs-$(CONFIG_ARMVA)	+= startup.o
objs-$(CONFIG_ARMVA)	+= switch_to.o
objs-$(CONFIG_ARMV4)	+= irq-v4.o
objs-$(CONFIG_ARMV4)	+= undef-v4.o
objs-$(CONFIG_ARMV6)	+= vector-v6.o
objs-$(CONFIG_ARMV7)	+= perf-v7.o
objs-$(CONFIG_ARMV7)	+= vector-v6.o
objs-$(CONFIG_ARMV7M)	+= entry-v7m.o
objs-$(CONFIG_ARMV7M)	+= switch-v7m.o


deps-y	:= \
	target.lds	\


subdirs-y			+= lib/
subdirs-y			+= mm/
subdirs-y			+= debug/
subdirs-$(CONFIG_SOC_AM335X)	+= soc-am335x/

subdirs-$(CONFIG_MACH_BEAGLEBONE)	+= mach-beaglebone/
subdirs-$(CONFIG_MACH_STM32)	+= mach-stm32/
