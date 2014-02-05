objs-y	:= \
	startup.o	\
	switch_to.o	\
	dump-stack.o	\
	irq.o		\
	context.o	\
	traps.o		\

objs-$(CONFIG_ARMV4)	+= undef-v4.o
objs-$(CONFIG_ARMV6)	+= undef-v6.o
objs-$(CONFIG_ARMV7)	+= undef-v6.o


deps-y	:= \
	target.lds	\


subdirs-y			+= lib/
subdirs-y			+= debug/
subdirs-$(CONFIG_MACH_BEAGLEBONE)	+= mach-beaglebone/
