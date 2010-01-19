#
# project specific configuration file
# to be placed in main project directory
#

# set default mode
export MODE:=default

# set default profile
export PROFILE:=test

# set default toolchain
export TC:=local

# enable ccache
export WITH_CCACHE:=1

# project-specific flags
export USER_OPT_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
export USER_DBG_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
export USER_PRF_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
