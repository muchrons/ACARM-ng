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

#
# work around for GCC 4.5/C++0x libs for intel (required when GCC>=4.5 is default one)
#
ifeq ($(TC),intel)
# BR-hack is required, since inserting brackets directly inside $(shell ) breaks syntax...
BR:=)
GCC_VER:=$(shell g++ --version | head -1 | sed "s:^[^$(BR)]\+$(BR) *::" | sed 's: .*::' | sed 's:^\([0-9]\+\.[0-9]\+\).*:\1:')
BR:=
ifeq ($(GCC_VER),4.5)
$(shell echo "--------------------------------------------------------------------------------------------------" >&2)
$(shell echo "           COMPILING WITH ICC, USING GCC 4.5 LIBS - THIS MAY LEAD TO PROBLEMS"                      >&2)
$(shell echo "WARNING: gcc 4.5 is (as for now) incompatible with icc, since it uses implicit c++0x in"            >&2)
$(shell echo "WARNING: header files (intel does not make such an assumption). program probably will not compile." >&2)
$(shell echo "WARNING: to fix this issue try installing gcc 4.4 or newer icc."                                    >&2)
$(shell echo "WARNING: you may want to use gcc 4.4 libs instead (via -I in config.mk)"                            >&2)
$(shell echo "--------------------------------------------------------------------------------------------------" >&2)
endif # gcc 4.5
endif # intel
