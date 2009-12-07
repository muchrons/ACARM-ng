# use Verbose mode on user request
ifeq (,$V)
  MFLAGS+=-s
endif

COMMON_FLAGS= -Wall -Wextra -pipe -I$(GEN_INCLUDES_DIR)
CFLAGS  += $(COMMON_FLAGS)
CXXFLAGS+= $(COMMON_FLAGS)
LDFLAGS += -L$(GEN_LIBS_DIR)

export MFLAGS \
       CFLAGS \
       CXXFLAGS \
       LDFLAGS

# turn memory debugging on, if requested
ifneq (,$(MEM_DEBUG))
  END_LINK_LIBS+=-lefence
  MEM_CHECK:=memdeb
else
  MEM_CHECK:=normal
endif
export END_LINK_LIBS \
       MEM_CHECK
