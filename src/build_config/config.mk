#
# project specific configuration file
# to be placed in main project directory
#

# set default mode
export MODE=default

# set default profile
export PROFILE=test

# set default toolchain
export TC=local

# enable distcc
#export WITH_DISTCC:=1

# features to build with
export FEATURES= \
                 lock_on_write_graph_add \
                 use_new_boost_filesystem

TMP:=
TMP_OPT:=
TMPLD:=

# enable ccache by default, if present in the system
ifneq (,$(shell which ccache))
export WITH_CCACHE:=1
endif

# work around for GCC 4.5/C++0x libs for intel (required when GCC>=4.5 is default one)
ifeq ($(TC),intel)
# BR-hack is required, since inserting brackets directly inside $(shell ) breaks syntax...
BR:=)
GCC_VER:=$(shell g++ --version | head -1 | sed "s:^[^$(BR)]\+$(BR) *::" | sed 's: .*::' | sed 's:^\([0-9]\+\.[0-9]\+\).*:\1:')
BR:=
ifeq ($(GCC_VER),4.5)
$(warning WARNING: compiling with ICC, using GCC 4.5 libs - this may lead to problems)
ifeq ($(shell test -e /usr/include/c++/4.4 && echo OK),OK)
$(warning WARNING: detected libs for GCC 4.4 - using these ones instead)
TMP+=-I /usr/include/c++/4.4
else
$(warning GCC 4.5 IS (AS FOR NOW) INCOMPATIBLE WITH ICC, SINCE IT USES IMPLICIT C++0x IN)
$(warning HEADER FILES (INTEL DOES NOT MAKE SUCH AN ASSUMPTION). PROGRAM PROBABLY WILL NOT COMPILE.)
$(warning TO FIX THIS ISSUE TRY INSTALLING GCC 4.4 OR NEWER ICC.)
endif # has gcc 4.4 libs
endif # gcc 4.5
endif # intel

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
