# compiler flags
OPT_FLAGS:=-O3 -DNDEBUG -Werror
DBG_FLAGS:=-g3
PRF_FLAGS:=$(DBG_FLAGS) $(OPT_FLAGS) -pg

# linker flags
PRF_LDFLAGS:=-pg
DBG_LDFLAGS:=
OPT_LDFLAGS:=
