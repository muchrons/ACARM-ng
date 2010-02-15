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
TMP:=-wd177,279,383,981,1418
else
TMP:=
endif
export USER_OPT_FLAGS:=-march=core2 $(TMP)
export USER_DBG_FLAGS:=$(TMP)
export USER_PRF_FLAGS:=$(USER_OPT_FLAGS)
# cleanup TMP
TMP:=
unexport TMP
