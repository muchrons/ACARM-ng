# don't use Verbose mode (be silent)?
ifeq (,$V)
MFLAGS+=-s
endif

COMMON_FLAGS=-Wall -Wextra -pipe -I$(GEN_INCLUDES_DIR) -L$(GEN_LIBS_DIR)
CFLAGS  +=$(COMMON_FLAGS)
CXXFLAGS+=$(COMMON_FLAGS)
LDFLAGS +=

export MFLAGS \
       CFLAGS \
       CXXFLAGS \
       LDFLAGS

