no-dot-config-targets := clean distclean help
include scripts/Makefile.main

.PHONY: _all
_all:


# We process the rest of the Makefile if this is the final invocation of make
ifndef skip-makefile

HOSTCC		:= gcc
HOSTCFLAGS	:= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer \
		-Wno-strict-aliasing \
		-Wno-unused-function \
		-Wno-unused-but-set-variable\


export	HOSTCC HOSTCFLAGS


ifdef	mixed-targets
# ===========================================================================
# We're called with mixed targets (*config and build targets).
# Handle them one by one.

%:: FORCE
	$(Q)$(MAKE) $@

else
ifdef	config-targets
# ===========================================================================
# *config targets only - make sure prerequisites are updated, and descend
# in scripts/kconfig to make the *config target

%config: FORCE
	$(Q)mkdir -p include/config
	$(Q)$(MAKE) -f scripts/Makefile.host curdir=scripts/kconfig $@

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
	$(Q)$(MAKE) -f Makefile silentoldconfig
else
# Dummy target needed, because used as prerequisite
include/config/auto.conf: ;

.PHONY: clean distclean
clean: FORCE
	@echo "CLEAN"
	$(Q)git clean -x -d -q -e .config -e scripts/

distclean: FORCE
	@echo "DISTCLEAN"
	@$(Q)git clean -x -d -q
endif	# no-dot-config

endif	# config-targets
endif	# mixed-targets


####

ifdef CONFIG_ARCH
CPPFLAGS-y	:= -Iinclude -include include/autoconf.h
CFLAGS-y	:= -include include/kernel.h
CFLAGS-y	+= -Wall -Wundef -Wno-trigraphs
CFLAGS-y	+= -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wimplicit
#CFLAGS-y	+= -Werror=missing-prototypes -Werror=missing-declarations
CFLAGS-y	+= -Wwrite-strings -Wunknown-pragmas -Wcast-qual -Wcast-align -Wsign-compare
CFLAGS-y	+= -Wno-unused-function
#CFLAGS-y	+= -Werror=implicit-function-declaration
CFLAGS-y	+= -nostdinc -ffreestanding
CFLAGS-y	+= -fno-ident
CFLAGS-y	+= -Os
CFLAGS-y	+= -g
CFLAGS-$(CONFIG_GC_SECTIONS) += -ffunction-sections -fdata-sections
AFLAGS-y	:=-D__ASSEMBLY__ -fverbose-asm

include arch/$(CONFIG_ARCH)/Makefile.arch
arch/$(CONFIG_ARCH)/Makefile.arch: ;

CFLAGS-y	+= ${ARCHFLAGS-y}
AFLAGS-y	+= ${ARCHFLAGS-y}


ifdef	CONFIG_CROSS_COMPILE
CROSS_COMPILE := $(CONFIG_CROSS_COMPILE)
endif

CC	:= $(CROSS_COMPILE)gcc
AR	:= $(CROSS_COMPILE)ar
OBJCOPY	:= $(CROSS_COMPILE)objcopy
READELF	:= $(CROSS_COMPILE)readelf
CPP	:= $(CC) -E

SYS_INCDIR := $(shell $(CC) -print-file-name=include)
CFLAGS-y   += -isystem $(SYS_INCDIR)
#######################################################################
subdirs-y			:= arch/$(CONFIG_ARCH)
subdirs-y			+= block
subdirs-y			+= drivers
subdirs-y			+= fs
subdirs-y			+= kapi
subdirs-y			+= kernel
subdirs-y			+= lib
subdirs-y			+= mm
subdirs-$(CONFIG_NET)		+= net

include scripts/Makefile.build
scripts/Makefile.build: prepare


subdirs:=$(subdirs-y)
libs :=
-include $(subdirs:%=%/.make)

libtarget.a: $(libs) FORCE
	@echo 'AR	$@'
	@rm -f $@	# avoid problems when removing some members
	$(Q)$(AR) crsT $@ $(filter %.a,$^)


_all: libtarget.a FORCE

symlinks := include/arch

mach-include := arch/$(CONFIG_ARCH)/mach-$(CONFIG_MACH)/include
has-mach-include := $(shell test -d $(mach-include) && echo y)
ifeq '$(has-mach-include)' 'y'
symlinks += include/mach
endif
ifdef CONFIG_SOC
symlinks += include/soc
endif
.PHONY: prepare
prepare: $(symlinks) $(asm-offsets)

include/arch: .config
	@echo "SYMLINK	$@"
	$(Q)ln -sfn ../arch/$(CONFIG_ARCH)/include $@
	@touch $@/$$; rm $@/$$	# force $@ dir to be older than .config
include/mach: SRC=arch/$(CONFIG_ARCH)/mach-$(CONFIG_MACH)/include
include/mach: .config
	@echo "SYMLINK	$@"
	$(Q) if [ -d "${SRC}" ]; then ln -sfn "../${SRC}" $@; touch $@/$$; rm $@/$$; fi
include/soc: SRC=arch/$(CONFIG_ARCH)/soc-$(CONFIG_SOC)/include
include/soc: .config
	@echo "SYMLINK	$@"
	$(Q) if [ -d "${SRC}" ]; then ln -sfn "../${SRC}" $@; touch $@/$$; rm $@/$$; fi

$(asm-offsets): $(symlinks)

namespacecheck:
	@scripts/namespace.pl

ifneq ($(filter tests/%,$(MAKECMDGOALS)),)
LDFLAGS+=-static -nostdlib
LDFLAGS-$(CONFIG_GC_SECTIONS) += -Wl,--gc-sections
LDFLAGS-$(CONFIG_GC_SECTIONS_SHOW) += -Wl,--print-gc-sections
ifeq ($(KBUILD_VERBOSE),2)
LDFLAGS+=-Wl,--verbose
endif

LDFLAGS+=$(LDFLAGS-y)

progs :=
-include tests/.make
pgms-deps := libtarget.a arch/$(CONFIG_ARCH)/target.lds
tests := $(filter tests/%.test,$(progs))
tests/: $(tests:%=%.bin)
$(progs): %: %.o $(pgms-deps) kernel/version.o
	@echo "LINK	$@"
	$(Q)$(CC) -Wl,--gc-sections					\
		-Wl,-Map,$@.map,--cref					\
		-Tarch/$(CONFIG_ARCH)/target.lds $(LDFLAGS) 		\
		-Wl,--start-group -Wl,--whole-archive libtarget.a -Wl,--no-whole-archive -Wl,--end-group	\
		-lgcc							\
		$< kernel/version.o -o $@
	@cp $@ kimage.elf

CONFIG_LOAD_OFFSET ?= ${CONFIG_TEXT_OFFSET}
LOADADDR:=$(shell printf 0x%08x $$((${CONFIG_PHYS_ADDR} + ${CONFIG_LOAD_OFFSET})))

tests/%.tftp: tests/%.$(CONFIG_IMG_FORMAT)
	$(Q)cp $< /tftpboot/$(CONFIG_MACH).$(CONFIG_IMG_FORMAT)

%.uimg: %.bin
	@echo "MKIMAGE	$@"
	$(Q)mkimage -A $(CONFIG_ARCH) -T kernel -O linux -C none -e ${LOADADDR} -a ${LOADADDR} -d $< $@
.SECONDARY: %.uimg

kernel/version.o: $(progs:%=%.o) $(pgms-deps)

endif	# tests/%

endif	# CONFIG_ARCH

endif	# skip-makefile

PHONY += FORCE
FORCE:

# Cancel implicit rules on top Makefile, `-rR' will apply to sub-makes.
Makefile: ;

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)
