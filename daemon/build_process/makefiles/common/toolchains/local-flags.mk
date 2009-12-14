# compiler flags
OPT_FLAGS:=-O3 -DNDEBUG -Werror
DBG_FLAGS:=-g3
PRF_FLAGS:=-g $(OPT_FLAGS) -pg

# linker flags
DBG_LDFLAGS:=-rdynamic
OPT_LDFLAGS:=-rdynamic
PRF_LDFLAGS:=$(OPT_LDFLAGS) -pg
