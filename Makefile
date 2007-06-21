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

# Basic helpers built in scripts/
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) -f scripts/Makefile.host obj=scripts/basic
# To avoid any implicit rule to kick in, define an empty command.
scripts/basic/%: scripts_basic ;

no-dot-config-targets := clean distclean help

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

%config: scripts_basic FORCE
	$(Q)mkdir -p include/config
	$(Q)$(MAKE) -f scripts/Makefile.host obj=scripts/kconfig $@

else
# ===========================================================================
# build targets only

# Additional helpers built in scripts/
PHONY += scripts
scripts: scripts_basic include/config/auto.conf

scripts_basic: include/autoconf.h

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
endif	# no-dot-config

endif	# config-targets
endif	# mixed-targets


####
subdirs-y			:= arch/$(CONFIG_ARCH)
subdirs-y			+= misc

subdirs:=$(subdirs-y)
objs :=
-include $(subdirs:%=%/.make)

libtarget.a: $(objs)
	@echo '  AR      $@'
	$(Q)$(AR) crs $@ $^


_all: scripts libtarget.a FORCE
	@echo "objs = $(objs)"

CC	:= arm-linux-gcc
CPP	:= $(CC) -E
CPPFLAGS:= -Iinclude -Iarch/$(CONFIG_ARCH)/include -include include/autoconf.h
CFLAGS	:= -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
CFLAGS	+= -Os 
include scripts/Makefile.build


endif	# skip-makefile

PHONY += FORCE
FORCE:

# Cancel implicit rules on top Makefile, `-rR' will apply to sub-makes.
Makefile: ;

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)
