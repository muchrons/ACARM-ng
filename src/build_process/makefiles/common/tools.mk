# include proper toolchain
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC).mk

export CC \
       CXX \
       AR \
       RANLIB \
       LD \
       STRIP \
       DOXYGEN

