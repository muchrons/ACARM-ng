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
# show warning when using ccache with intel compiler
#
ifeq ($(TC),intel)
ifneq (,$(WITH_CCACHE))
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
 $(shell echo "                 USING INTEL's TOOLCHAIN WITH CCACHE ENABLED"                        >&2)
 $(shell echo "WARNING: Intel's compiler is broken - it does not work well with ccache enabled."    >&2)
 $(shell echo "WARNING: your should disable it in build_config/config.mk file. otherwise you might" >&2)
 $(shell echo "WARNING: get strange errors from make, about being unable to generate dependency"    >&2)
 $(shell echo "WARNING: for */.ccache/*.ii files, etc..."                                           >&2)
 $(shell echo "WARNING: you have been warned."                                                      >&2)
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
endif
endif
