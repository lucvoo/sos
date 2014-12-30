objs-y	+= 	context.o
objs-y	+= 	dump-stack.o
objs-y	+= 	init.o
objs-y	+= 	startup.o
objs-y	+= 	switch_to.o


objs-$(CONFIG_ARMV4)	+= irq-v4.o
objs-$(CONFIG_ARMV4)	+= undef-v4.o
objs-$(CONFIG_ARMV6)	+= vector-v6.o
objs-$(CONFIG_ARMV7)	+= perf-v7.o
objs-$(CONFIG_ARMV7)	+= vector-v6.o


deps-y	:= \
	target.lds	\


subdirs-y			+= lib/
subdirs-y			+= mm/
subdirs-y			+= debug/
subdirs-y			+= socs/
subdirs-$(CONFIG_MACH_BEAGLEBONE)	+= mach-beaglebone/
