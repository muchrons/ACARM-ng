/*
 * isFileSane.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Commons/Filesystem/isFileSane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  ~TestClass(void)
  {
    remove("test_fifo");
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/isFileSane");
} // unnamed namespace


namespace tut
{

// test if sane file is marked as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("normal file not sane", isFileSane("commons.t") );
}

// test if check for non-exisitng file fails
template<>
template<>
void testObj::test<2>(void)
{
  ensure("non-existing file marked sane", isFileSane("some/non/existing/file")==false );
}

// test if link to file is not sane
template<>
template<>
void testObj::test<3>(void)
{
  ensure("link file marked sane", isFileSane("testdata/fileSymlink")==false );
}

// test if dangling symlink throws
template<>
template<>
void testObj::test<4>(void)
{
  ensure("dangling symlink marked sane", isFileSane("testdata/danglingSymlink")==false );
}

// test if throws on dir
template<>
template<>
void testObj::test<5>(void)
{
  ensure("dir marked sane", isFileSane("testdata/dir")==false );
}

// test if links in path are mathed as well
template<>
template<>
void testObj::test<6>(void)
{
  ensure("link inside path marked sane", isFileSane("testdata/dirSymlink/other_dir/some_file")==false );
}

// test some global file
template<>
template<>
void testObj::test<7>(void)
{
  ensure("global file not marked sane", isFileSane("/etc/passwd") );
}

// test some relative file path
template<>
template<>
void testObj::test<8>(void)
{
  ensure("relative file not marked sane", isFileSane("testdata/file") );
}

// test if fifo is NOT sane file
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("unable to create fifo", mkfifo("test_fifo", 0600), 0);
  ensure("fifo marked sane", isFileSane("test_fifo")==false );
}

} // namespace tut
