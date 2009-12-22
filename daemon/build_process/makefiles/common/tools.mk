# include proper toolchain
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC).mk

# ccache support, if requested
ifneq (,$(WITH_CCACHE))
  CCACHE:=ccache
  CC    :=$(CCACHE) $(CC)
  CXX   :=$(CCACHE) $(CXX)
endif

export CC \
       CXX \
       AR \
       RANLIB \
       NM \
       LD \
       STRIP \
       DOXYGEN
