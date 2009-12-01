# compiler flags
OPT_FLAGS:=-O3 -DNDEBUG -Werror
DBG_FLAGS:=-g3 -rdynamic
PRF_FLAGS:=$(DBG_FLAGS) $(OPT_FLAGS) -pg

# linker flags
DBG_LDFLAGS:=
OPT_LDFLAGS:=
PRF_LDFLAGS:=$(DBG_LDFLAGS) $(OPT_LDFLAGS) -pg
