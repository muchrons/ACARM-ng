#
# show warning when using ccache with old intel compiler
#
ifneq (,$(WITH_CCACHE))
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC)/tools.mk
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
