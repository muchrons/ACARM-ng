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

# enable ccache, if not using intel's toolchain
ifneq ($(TC),intel)
export WITH_CCACHE=1
endif

# project-specific flags
# for intel disable some annoying remarks
ifeq ($(TC),intel)
TMP:=
TMP+=-wd193	# zero used for undefined preprocessing identifier
TMP+=-wd279	# controlling expression is constant
TMP+=-wd383	# value copied to temporary, reference to temporary used
TMP+=-wd981	# operands are evaluated in unspecified order
TMP+=-wd1418	# external function definition with no prior declaration
TMPLD:=-lstdc++
else
TMP:=
TMPLD:=
endif
export USER_OPT_FLAGS:=-march=core2 $(TMP)
export USER_DBG_FLAGS:=$(TMP)
export USER_PRF_FLAGS:=$(USER_OPT_FLAGS)
export USER_OPT_LDFLAGS:=$(TMPLD)
export USER_DBG_LDFLAGS:=$(TMPLD)
export USER_PRF_LDFLAGS:=$(TMPLD)
# cleanup TMP
TMP:=
TMPLD:=
unexport TMP
unexport TMPLD
