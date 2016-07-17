objs-y	+= cache.o
objs-y	+= cache-all.o
objs-y	+= context.o
objs-y	+= debug/dump_system_regs.o
objs-y	+= dump-stack.o
objs-y	+= exception.o
objs-y	+= mmu-setup.o
objs-y	+= perf.o
objs-y	+= startup.o
objs-y	+= switch_to.o
objs-y	+= vectors.o

objs-$(CONFIG_SMP)	+= smp.o

deps-y	:= \
	target.lds	\


subdirs-y			+= mm/


subdirs-y			+= soc-$(CONFIG_SOC)/
subdirs-y			+= mach-$(CONFIG_MACH)/
