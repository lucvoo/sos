no-dot-config-targets := clean distclean help
include scripts/Makefile.main

.PHONY: _all
_all:


# We process the rest of the Makefile if this is the final invocation of make
ifndef skip-makefile

srctree		:= $(CURDIR)
objtree		:= $(CURDIR)
obj		:= $(objtree)

VPATH		:= $(srctree)

export srctree objtree VPATH


HOSTCC		:= gcc
HOSTCFLAGS	:= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer

export	HOSTCC HOSTCFLAGS


# Look for make include files relative to root of kernel src
MAKEFLAGS += --include-dir=$(srctree)


ifdef	mixed-targets
# ===========================================================================
# We're called with mixed targets (*config and build targets).
# Handle them one by one.

%:: FORCE
	$(Q)$(MAKE) -C $(srctree) KBUILD_SRC= $@

else
ifdef	config-targets
# ===========================================================================
# *config targets only - make sure prerequisites are updated, and descend
# in scripts/kconfig to make the *config target

%config: FORCE
	$(Q)mkdir -p include/config
	$(Q)$(MAKE) -f scripts/Makefile.host obj=scripts/kconfig $@

else
# ===========================================================================
# build targets only

ifndef	no-dot-config
# Read in config
-include include/config/auto.conf
-include include/config/auto.conf.cmd

# To avoid any implicit rule to kick in, define an empty command
.config include/config/auto.conf.cmd: ;

# If .config is newer than include/config/auto.conf, someone tinkered
# with it and forgot to run make oldconfig.
# if auto.conf.cmd is missing then we are probably in a cleaned tree so
# we execute the config step to be sure to catch updated Kconfig files
include/config/auto.conf: .config include/config/auto.conf.cmd
	$(Q)$(MAKE) -f $(srctree)/Makefile silentoldconfig
else
# Dummy target needed, because used as prerequisite
include/config/auto.conf: ;

.PHONY: clean distclean
clean: FORCE
	$(Q)$(MAKE) -f scripts/Makefile.clean obj=tests
	$(Q)find * -name '*.[oas]' -o -name '.*.cmd' -o -name '.*.d' -o -name '*.lds' -type f | xargs rm -f

distclean-files := .config .config.old include/autoconf.h
distclean-files	+= include/arch/asm-offsets.h
distclean-files	+= include/arch/mach
distclean-files += include/arch include/mach
distclean-dirs	:= include/config/
distclean: clean FORCE
	$(Q)$(MAKE) -f scripts/Makefile.clean obj=scripts/kconfig
	$(Q)$(MAKE) -f scripts/Makefile.clean obj=scripts/basic
	$(Q)find */ -name '.make' -type f | xargs rm -f
	@echo "CLEAN	$(distclean-files)"
	@rm -rf $(distclean-files)
	@echo "CLEAN	$(distclean-dirs)"
	@rm -rf $(distclean-dirs)
endif	# no-dot-config

endif	# config-targets
endif	# mixed-targets


####

ifdef CONFIG_ARCH
CPPFLAGS-y	:= -Iinclude -include include/autoconf.h
CFLAGS-y	:= -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
CFLAGS-y	+= -Os 
CFLAGS-$(CONFIG_GC_SECTIONS) += -ffunction-sections -fdata-sections
AFLAGS-y	:=-D__ASSEMBLY__ -fverbose-asm

include arch/$(CONFIG_ARCH)/Makefile.arch
arch/$(CONFIG_ARCH)/Makefile.arch: ;

CC	:= $(CROSS_COMPILE)gcc
AR	:= $(CROSS_COMPILE)ar
OBJCOPY	:= $(CROSS_COMPILE)objcopy
CPP	:= $(CC) -E

#######################################################################
subdirs-y			:= arch/$(CONFIG_ARCH)
subdirs-y			+= misc
subdirs-y			+= kernel
subdirs-y			+= kapi

include scripts/Makefile.build
scripts/Makefile.build: prepare


subdirs:=$(subdirs-y)
objs :=
-include $(subdirs:%=%/.make)

libtarget.a: $(objs)
	@echo 'AR	$@'
	$(Q)$(AR) crs $@ $^


_all: libtarget.a FORCE

symlinks:= include/arch include/mach include/arch/mach
.PHONY: prepare
prepare: $(symlinks) $(asm-offsets)

include/arch: .config
	@echo "SYMLINK	$@"
	$(Q)ln -sfn ../arch/$(CONFIG_ARCH)/include $@
	@touch $@/$$; rm $@/$$	# force $@ dir to be older than .config
include/mach: .config
	@echo "SYMLINK	$@"
	$(Q)ln -sfn ../arch/$(CONFIG_ARCH)/mach-$(CONFIG_MACH)/include $@
	@touch $@/$$; rm $@/$$	# force $@ dir to be older than .config
include/arch/mach: include/arch
	@echo "SYMLINK	$@"
	$(Q)ln -sfn ../mach-$(CONFIG_MACH)/include $@
	@touch $@/$$; rm $@/$$	# force $@ dir to be older than .config

$(asm-offsets): $(symlinks)

endif	# CONFIG_ARCH

ifneq ($(filter tests/%,$(MAKECMDGOALS)),)
LDFLAGS+=-static -nostdlib
LDFLAGS-$(CONFIG_GC_SECTIONS) += -Wl,--gc-sections
LDFLAGS-$(CONFIG_GC_SECTIONS_SHOW) += -Wl,--print-gc-sections
ifeq ($(KBUILD_VERBOSE),2)
LDFLAGS+=-Wl,--verbose
endif

LDFLAGS+=$(LDFLAGS-y)

misc/version.o: FORCE

include tests/Makefile
tests=$(progs-y:%=tests/%)
tests/: $(tests:%=%.bin)
$(tests): %: %.o libtarget.a misc/version.o arch/$(CONFIG_ARCH)/target.lds
	@echo "LINK	$@"
	$(Q)$(CC) -Wl,--gc-sections					\
		-Wl,-Map,$@.map,--cref					\
		-Tarch/$(CONFIG_ARCH)/target.lds $(LDFLAGS) 		\
		misc/version.o						\
		-Wl,--start-group -Wl,--whole-archive libtarget.a -Wl,--no-whole-archive -Wl,--end-group	\
		-lgcc							\
		$< -o $@
endif	# tests/%

endif	# skip-makefile

PHONY += FORCE
FORCE:

# Cancel implicit rules on top Makefile, `-rR' will apply to sub-makes.
Makefile: ;

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)
