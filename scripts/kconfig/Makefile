# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

CFG_ARCH ?= $(CONFIG_ARCH)

PHONY += oldconfig menuconfig config silentoldconfig

menuconfig: $(obj)/mconf
	$(Q)$< Kconfig

consoleconfig: $(obj)/conf
	$(Q)$< Kconfig

oldconfig: $(obj)/conf
	$(Q)$< -o Kconfig

silentoldconfig: $(obj)/conf
	$(Q)$< -s Kconfig


PHONY += randconfig allyesconfig allnoconfig defconfig

randconfig: $(obj)/conf
	$(Q)$< -r Kconfig

allyesconfig: $(obj)/conf
	$(Q)$< -y Kconfig

allnoconfig: $(obj)/conf
	$(Q)$< -n Kconfig

defconfig: $(obj)/conf
	$(Q)$< -D arch/$(CFG_ARCH)/configs/defconfig Kconfig

%_config: $(obj)/conf
	$(Q)$< -D arch/$(CFG_ARCH)/configs/$@ Kconfig

%_defconfig: $(obj)/conf
	$(Q)rm -f include/arch include/mach include/soc
	$(Q)rm -f include/config/auto.conf include/config/auto.conf.cmd
	$(Q)rm -f include/autoconf.h
	$(Q)$< -D arch/$*/configs/defconfig Kconfig

# Help text used by make help
help:
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	@echo  '  silentoldconfig - Same as oldconfig, but quietly'
	@echo  '  randconfig	  - New config with random answer to all options'
	@echo  '  defconfig	  - New config with default answer to all options'
	@echo  '  allyesconfig	  - New config where all options are accepted with yes'
	@echo  '  allnoconfig	  - New config where all options are answered with no'


# Use reursively expanded variables so we do not call gcc unless
# we really need to do so. (Do not call gcc as part of make mrproper)
HOST_EXTRACFLAGS = -DCURSES_LOC="<ncurses.h>"
HOST_LOADLIBES   = -lncurses
HOST_EXTRACFLAGS += -DLOCALE
HOST_EXTRACFLAGS += -DKBUILD_NO_NLS


# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# mconf:  Used for the mconfig target.
#         Utilizes the lxdialog package
# object files used by all kconfig flavours

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

conf-objs	:= conf.o  zconf.tab.o
mconf-objs	:= mconf.o zconf.tab.o $(lxdialog)

hostprogs-y := conf
hostprogs-y += mconf

$(obj)/zconf.tab.o: $(obj)/lex.zconf.c $(obj)/zconf.hash.c

$(obj)/kconfig_load.o: $(obj)/lkc_defs.h

$(obj)/lkc_defs.h: $(src)/lkc_proto.h
	sed < $< > $@ 's/P(\([^,]*\),.*/#define \1 (\*\1_p)/'

# ===========================================================================
%.tab.c: %.y
	bison -l -b $* -p $(notdir $*) $<

lex.%.c: %.l
	flex -L -P$(notdir $*) -o$@ $<
	rm -f lex.backup

%.hash.c: %.gperf
	gperf < $< > $@
