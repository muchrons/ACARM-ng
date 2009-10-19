PROGRAM_NAME=program
TEST_PROGRAM_NAME=tests
export PROGRAM_NAME \
       TEST_PROGRAM_NAME

ifeq (,$(filter %/gen/$(TARGET),$(basename $(CURDIR))))

# don't use Verbose mode (be silent)?
ifeq (,$V)
MFLAGS+=-s
endif

define run-in-gen
	@echo
	@echo "##### build $@"
	mkdir -p `find . -type d | grep -v './\.' | \
	          grep -v '^\./gen/' | grep -v '^\./gen$$' | \
			  grep -v '^\.$$' | sed 's:^\./:$(GENDIR)/$@/:'`
	+$(MAKE) $(MFLAGS) -C "$(GENDIR)/$@" -f "$(CURDIR)/Makefile" \
		TARGET=$@ $(TARGET)
endef

.SUFFIXES:


# if gendir was not set by upper call of make, set it now
ifeq (,$(GENDIR))
GENDIR=$(CURDIR)/gen
endif
SRCDIR=$(CURDIR)
VPATH+=$(SRCDIR)
export GENDIR SRCDIR VPATH


CXXSRCS_NOMAIN=$(shell find . -iname '*.cpp' -exec grep -L '^int main' {} \; )
CSRCS_NOMAIN  =$(shell find . -iname '*.c'   -exec grep -L '^int main' {} \; )
export CSRCS_NOMAIN CXXSRCS_NOMAIN
CXXTESTSRCS=$(shell find . -iname '*.t.cpp')
CTESTSRCS  =$(shell find . -iname '*.t.c'  )
export CTESTSRCS CXXTESTSRCS

CXXSRCS=$(filter-out $(CXXTESTSRCS), $(shell find . -iname '*.cpp'))
CSRCS  =$(filter-out $(CTESTSRCS),   $(shell find . -iname '*.c'  ))
export CSRCS CXXSRCS


CC=gcc
CFLAGS+=-Wall
export CC CFLAGS

CXX=g++
CXXFLAGS+=-Wall
export CXX CXXFLAGS

export LDFLAGS

export TARGET

# these will be set in child-call:
export LIBS LIBS_TEST


.PHONY: all
all: debug

.PHONY: debug
debug: CXXFLAGS+=-g3
debug: TARGET=all
debug:
	@$(run-in-gen)

.PHONY: release
release: CXXFLAGS+=-O3 -DNDEBUG
release: TARGET=all
release:
	@$(run-in-gen)

.PHONY: profile
profile: CXXFLAGS+=-pg -DNDEBUG
profile: LDFLAGS+=-pg
profile: TARGET=all
profile:
	@$(run-in-gen)

.PHONY: test
test: CXXFLAGS+=-g3
test: TARGET=test
test:
	@$(run-in-gen)

.PHONY: clean
clean:
	@echo
	@echo "##### clean"
	rm -rvf "$(GENDIR)"

else

# object files:
COBJS  +=$(CSRCS:.c=.o)
CXXOBJS+=$(CXXSRCS:.cpp=.o)
# obj files without 'main()':
COBJS_NOMAIN  +=$(CSRCS_NOMAIN:.c=.o)
CXXOBJS_NOMAIN+=$(CXXSRCS_NOMAIN:.cpp=.o)
# test object files:
CTESTOBJS  +=$(CTESTSRCS:.c=.o)
CXXTESTOBJS+=$(CXXTESTSRCS:.cpp=.o)

# dep files:
CDEPS  +=$(CSRCS:.c=.d)     $(CTESTSRCS:.c=.d)
CXXDEPS+=$(CXXSRCS:.cpp=.d) $(CXXTESTSRCS:.cpp=.d)


.PHONY: all
all: $(PROGRAM_NAME)

.PHONY: test
test: $(TEST_PROGRAM_NAME)

$(TEST_PROGRAM_NAME): $(CXXTESTOBJS) $(CTESTOBJS) \
						$(CXXOBJS_NOMAIN) $(COBJS_NOMAIN)
	echo "LD  $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PROGRAM_NAME): $(CXXOBJS) $(COBJS)
	echo "LD  $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

# C++ dep files:
%.d: %.cpp
	echo "DEP $*.cpp"
	$(CXX) -MM $(CXXFLAGS) "$(SRCDIR)/$*.cpp" > "$@"
	sed -i 's#\($*\).o[ :]*#\1.o $@: #' "$@"

# C++ source files:
%.o: %.cpp
	echo "CXX $*.cpp"
	$(CXX) $(CXXFLAGS) -c "$(SRCDIR)/$*.cpp" -o "$@"

# C dep files:
%.d: %.c
	echo "DEP $*.c"
	$(CC) -MM $(CFLAGS) "$(SRCDIR)/$*.c" > "$@"
	sed -i 's#\($*\).o[ :]*#\1.o $@: #' "$@"

# C source files:
%.o: %.c
	echo "CC  $*.c"
	$(CC) $(CFLAGS) -c "$(SRCDIR)/$*.c" -o "$@"


# has C sources?
ifneq (,$(CDEPS))
-include $(CDEPS)
endif

# has C++ sources?
ifneq (,$(CXXDEPS))
-include $(CXXDEPS)
endif

endif

