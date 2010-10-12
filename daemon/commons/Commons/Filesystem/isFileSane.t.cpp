/*
 * isFileSane.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/isFileSane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  template<typename TEx>
  void ensureThrow(const path &p) const
  {
    try
    {
      isFileSane(p);   // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
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
  ensureThrow<ExceptionFilesystemIO>("some/non/existing/file");
}

// test if link to file is not sane
template<>
template<>
void testObj::test<3>(void)
{
  ensure("link file marked sane", isFileSane("testdata/fileSymlink")==false );
}

// test if dongling symlink throws
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/donglingSymlink");
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

} // namespace tut
