# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

CFG_ARCH ?= $(CONFIG_ARCH)
VPATH := $(curdir)

PHONY += oldconfig menuconfig config silentoldconfig

menuconfig: mconf
	$(Q)$< Kconfig

consoleconfig: conf
	$(Q)$< Kconfig

oldconfig: conf
	$(Q)$< -o Kconfig

silentoldconfig: conf
	$(Q)$< -s Kconfig


PHONY += randconfig allyesconfig allnoconfig defconfig

randconfig: conf
	$(Q)$< -r Kconfig

allyesconfig: conf
	$(Q)$< -y Kconfig

allnoconfig: conf
	$(Q)$< -n Kconfig

%_config: conf
	$(Q)rm -f include/mach include/soc
	$(Q)rm -f include/config/auto.conf include/config/auto.conf.cmd
	$(Q)rm -f include/autoconf.h
	$(Q)$< -D arch/$(CFG_ARCH)/configs/$@ Kconfig

%_defconfig: conf
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

$(curdir)/zconf.tab.o: $(curdir)/lex.zconf.c $(curdir)/zconf.hash.c

# ===========================================================================
%.tab.c: %.y
	bison -l -b $* -p $(notdir $*) $<

lex.%.c: %.l
	flex -L -P$(notdir $*) -o$@ $<
	rm -f lex.backup

%.hash.c: %.gperf
	gperf < $< > $@
