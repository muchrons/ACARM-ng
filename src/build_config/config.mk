#
# can be used to locally enable some specific settings/workarounds
#
-include build_config/local-config-pre.mk

#
# project specific configuration file
# to be placed in main project directory
#

# set default mode
export MODE:=default

# set default profile
export PROFILE:=test

# set default toolchain
export TC:=gcc

# enable distcc
#export WITH_DISTCC:=1

# features to build with
export FEATURES:= \
                 lock_on_write_graph_add \
                 use_old_boost_filesystem

TMP:=
TMP_OPT:=
TMPLD:=

# enable ccache by default, if present in the system
ifneq (,$(shell which ccache))
export WITH_CCACHE:=1
endif

# enable stack-protector
ifeq ($(TC),local)
TMP+=-fstack-protector-all
endif
ifeq ($(TC),intel)
TMP+=-fstack-security-check -fstack-protector
endif

# determine architecture to compile for
ARCH_STR:=$(shell uname -m)
ifeq ($(ARCH_STR),x86_64)
TMP+=-m64
else
TMP+=-m32
endif

# project-specific flags
# for intel disable some annoying remarks
ifeq ($(TC),intel)
TMP+=-wd193  # zero used for undefined preprocessing identifier
TMP+=-wd279  # controlling expression is constant
TMP+=-wd377  # class "XYZ" has no suitable copy constructor
TMP+=-wd383  # value copied to temporary, reference to temporary used
TMP+=-wd444  # destructor for base class "XYZ" is not virtual
TMP+=-wd654  # overloaded virtual function "ABC" is only partially overridden in class "XYZ"
TMP+=-wd814  # template nesting depth does not match the previous declaration of class template "XYZ"
TMP+=-wd819  # template nesting depth does not match the previous declaration of class template "XYZ"
TMP+=-wd981  # operands are evaluated in unspecified order
TMP+=-wd1125 # function "XYZ" -- virtual function override intended?
TMP+=-wd2165 # declaring a reference with "mutable" is nonstandard
TMPLD+=-lstdc++
else
#TMP:=
#TMPLD:=
endif

# turn compiuler-specific optimizations
ifeq ($(TC),gcc)
TMP_OPT+=
endif
ifeq ($(TC),intel)
TMP_OPT+=
endif

# these options can be given as a command line argument to make
# (feature used by ./configure script for extra libs places)
TMP+=$(EXTRA_COMPILE_FLAGS)
TMPLD+=$(EXTRA_LINK_FLAGS)

export USER_OPT_FLAGS:=$(TMP) $(TMP_OPT)
export USER_DBG_FLAGS:=$(TMP)
export USER_PRF_FLAGS:=$(USER_OPT_FLAGS)
export USER_OPT_LDFLAGS:=$(TMPLD)
export USER_DBG_LDFLAGS:=$(TMPLD)
export USER_PRF_LDFLAGS:=$(TMPLD)
# cleanup TMP
TMP:=
TMP_OPT:=
TMPLD:=
unexport TMP
unexport TMP_OPT
unexport TMPLD

#
# workarround for a missing defines in older intel compilers
#
ifeq ($(TC),intel)
ifneq ($(ENABLE_SIZEOF_MACROS_WORKAROUND),)
TMP:=
TMP+=-D__SIZEOF_INT__=sizeof\(int\)
TMP+=-D__SIZEOF_LONG__=sizeof\(long\)
#TMP+=-D__SIZEOF_LONG_LONG__=sizeof\(long\ long\)
TMP+=-D__SIZEOF_SHORT__=sizeof\(short\)
TMP+=-D__SIZEOF_POINTER__=sizeof\(void\*\)
TMP+=-D__SIZEOF_FLOAT__=sizeof\(float\)
TMP+=-D__SIZEOF_DOUBLE__=sizeof\(double\)
#TMP+=-D__SIZEOF_LONG_DOUBLE__=sizeof\(long\ double\)
TMP+=-D__SIZEOF_SIZE_T__=sizeof\(size_t\)

USER_OPT_FLAGS+=$(TMP)
USER_DBG_FLAGS+=$(TMP)
USER_PRF_FLAGS+=$(TMP)

export USER_OPT_FLAGS
export USER_DBG_FLAGS
export USER_PRF_FLAGS

TMP:=
endif # if need __SIZEOF_* workaround
endif # if intel

#
# may need to overwite some deafult configs for other hosts
#
-include build_config/local-config-post.mk
