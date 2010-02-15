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
TMP:=-wd177,279,383,424,981,1418
TMPLD:=-lstdc++
# these are ok.
TMP+=-wd271,444,522,819,854,1125,1572,1599,2259
else
TMP:=
TMPLD:=
endif
TMP+=-D__STRICT_ANSI__=0
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
