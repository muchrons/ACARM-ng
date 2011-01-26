#
# is any profile set?
#
PROFILES_LIST:=$(shell cd "$(MAKEFILES_PROFILES_BASE_DIR)" && ls | sed 's:\.mk::g' | xargs echo)
ifneq ($(PROFILE),$(findstring $(PROFILE),$(PROFILES_LIST)))
$(error PROFILE not set (aviable are: $(PROFILES_LIST)))
endif # PROFILE not set

#
# is any toolchain set?
#
TOOLCHAINS_LIST:=$(shell cd "$(MAKEFILES_TOOLCHAINS_BASE_DIR)" && ls | grep -v -- '-flags.mk' | sed 's:\.mk::g' | xargs echo)
ifneq ($(TC),$(findstring $(TC),$(TOOLCHAINS_LIST)))
$(error toolchain (TC) not set (aviable are: $(TOOLCHAINS_LIST)))
endif # TC not set

#
# is any mode set?
#
ifeq (,$(MODE))
$(error mode (MODE) not set)
endif # TC not set

#
# show warning when using ccache with old intel compiler
#
ifeq ($(TC),intel)
ifneq (,$(WITH_CCACHE))
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC).mk
# this warning is true only for ICC releaseses < 12.0.0
ifneq (ok,$(shell $(CC) -dumpversion | sed 's:\..*::' | xargs test 12 -le && echo 'ok'))
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
 $(shell echo "                 USING INTEL's TOOLCHAIN WITH CCACHE ENABLED"                        >&2)
 $(shell echo "WARNING: Intel's compiler in version before 12.0.0 is broken - it does not work"     >&2)
 $(shell echo "WARNING: well with ccache enabled. your should disable it in build_config/config.mk" >&2)
 $(shell echo "WARNING: file or upgrade to version 12.0.0 or newer. otherwise you might get"        >&2)
 $(shell echo "WARNING: strange errors from make, about being unable to generate dependency for"    >&2)
 $(shell echo "WARNING: */.ccache/*.ii files, etc..."                                               >&2)
 $(shell echo "WARNING: you have been warned."                                                      >&2)
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
endif # too old (i.e. buggy) icc version
endif # with ccache
endif # icc

#
# does given features exist?
#
ALL_EXISTING_FEATURES_LIST:=$(shell for c in `cat $(COMPONENTS_LISTS) 2>/dev/null` ; do ls "$$c/features/modes/features" 2>/dev/null ; done )
NON_EXISTING_FEATURES_LIST:=$(filter-out $(ALL_EXISTING_FEATURES_LIST),$(FEATURES))
ifneq ($(NON_EXISTING_FEATURES_LIST),)
$(error following features does NOT exist: $(NON_EXISTING_FEATURES_LIST) (NOTE: available are: $(ALL_EXISTING_FEATURES_LIST)))
endif
