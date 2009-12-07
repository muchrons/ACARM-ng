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
TOOLCHAINS_LIST:=$(shell cd "$(MAKEFILES_TOOLCHAINS_BASE_DIR)" && ls | sed 's:\.mk::g' | xargs echo)
ifneq ($(TC),$(findstring $(TC),$(TOOLCHAINS_LIST)))
$(error toolchain (TC) not set (aviable are: $(TOOLCHAINS_LIST)))
endif # TC not set

#
# is any mode set?
#
ifeq (,$(MODE))
$(error mode (MODE) not set)
endif # TC not set
