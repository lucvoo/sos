objs-y	:= \
	startup.o	\
	switch_to.o	\
	dump-stack.o	\
	context.o	\

objs-$(CONFIG_ARMV4)	+= undef-v4.o
objs-$(CONFIG_ARMV4)	+= irq-v4.o
objs-$(CONFIG_ARMV6)	+= vector-v6.o
objs-$(CONFIG_ARMV7)	+= vector-v6.o


deps-y	:= \
	target.lds	\


subdirs-y			+= lib/
subdirs-y			+= debug/
subdirs-$(CONFIG_MACH_BEAGLEBONE)	+= mach-beaglebone/
