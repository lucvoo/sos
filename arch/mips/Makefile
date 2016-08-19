objs-y	+= 	cache.o
objs-y	+= 	cache-init.o
objs-y	+= 	context.o
objs-y	+= 	dma-alloc.o
objs-y	+= 	dma-mapping.o
objs-y	+= 	dump-stack.o
objs-y	+= 	init.o
objs-y	+= 	iomap.o
objs-y	+= 	startup.o
objs-y	+= 	switch_to.o
objs-y	+= 	traps.o
objs-y	+= 	vectors.o

objs-$(CONFIG_MIPS_CM)	+= cm.o
objs-$(CONFIG_SMP)	+= smp-entry.o

deps-y	:= \
	target.lds	\

subdirs-y			+= debug/

subdirs-$(CONFIG_SOC_AR933X)	+= soc-ar933x/
subdirs-$(CONFIG_MACH_OMEGA)	+= mach-omega/

subdirs-$(CONFIG_SOC_CXT200)	+= soc-cxt200/

subdirs-$(CONFIG_SOC_JZ4780)	+= soc-jz4780/
subdirs-$(CONFIG_MACH_CI20)	+= mach-ci20/
