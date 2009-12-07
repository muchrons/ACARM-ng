files indicating dependencies can be present in direcotry. general rules are:is executable and displays on the screeo
  - each file name represents profile-dependent elements. exception is 'common',
    which is taken as a part for all of the profiles.
  - {$profile|common} file lists components (one each line) that current one
    depends on. only components build by this build system are listed there!
  - {$profile|common}_dynamic_compile is executable and displays on the screen
    all flags that are to be taken for compilation of this component. common
    usage is adding system libraries (ex.: 'echo -lsome_sys_library') and
    getting dependencies in runtime, by means of dpkg-config and simillar
    tools. in general you're free to do whatever you wan inside this script
    as long as output contains one line with compiler flags.
  - {$profile|common}_dynamic_link is the same as corresponding
    {$profile|common}_dynamic_compile file, except that it provides flags
    that are to be passed for linker.

example file names that are supported are:

common
common_dynamic_compile
common_dynamic_link
debug
debug_dynamic_compile
debug_dynamic_link
mtest
mtest_dynamic_compile
mtest_dynamic_link
profile
profile_dynamic_compile
profile_dynamic_link
release
release_dynamic_compile
release_dynamic_link
test
test_dynamic_compile
test_dynamic_link
