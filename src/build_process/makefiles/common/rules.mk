
.SUFFIXES:

# C++ source files:
%.o:: %.cpp
	@echo "CXX   $*.cpp"
	$(CXX) $(CXXFLAGS) -MMD -c "$(THIS_SRC_BASE_DIR)/$*.cpp" -o "$@"

# C source files:
%.o:: %.c
	@echo "CC    $*.c"
	$(CC) $(CFLAGS)    -MMD -c "$(THIS_SRC_BASE_DIR)/$*.c"   -o "$@"

