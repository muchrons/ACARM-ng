.SUFFIXES:

# C++ source files:
%.oxx:: %.cpp
	@echo "CXX   $*.cpp"
	$(CXX) $(CXXFLAGS) -MMD -MF "$@.dxx" -c "$(THIS_SRC_BASE_DIR)/$*.cpp" -o "$@"

# C source files:
%.o:: %.c
	@echo "CC    $*.c"
	$(CC) $(CFLAGS)    -MMD -MF "$@.d"   -c "$(THIS_SRC_BASE_DIR)/$*.c"   -o "$@"

# missing headers on dependency problem - C++
%.hpp:
	@true

# missing headers on dependency problem - C
%.h:
	@true
