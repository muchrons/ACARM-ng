/*
 * createTempDirectory.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/createTempDirectory.hpp"

using namespace std;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  template<typename TEx>
  void ensureThrow(const Base::Filesystem::Path &p) const
  {
    try
    {
      createTempDirectory(p);   // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }

  void ensureOk(const Base::Filesystem::Path &p) const
  {
    const Base::Filesystem::Path out=createTempDirectory(p);
    tut::ensure("not created", exists(out) );
    tut::ensure("dir not created", is_directory(out) );
    remove(out);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/createTempDirectory");
} // unnamed namespace


namespace tut
{

// test throw when directory has no write permission
template<>
template<>
void testObj::test<1>(void)
{
  ensureThrow<ExceptionFilesystemIO>("/i_cannot_create_this_dir");
}

// test exception when directory is to be created inside dir with links in path
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/dirSymlink/other_dir");
}

// try creating some dir in current directory
template<>
template<>
void testObj::test<3>(void)
{
  ensureOk(".");
}

// test creating directory within some dir
template<>
template<>
void testObj::test<4>(void)
{
  ensureOk("testdata/dir");
}

// test empty path
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow<ExceptionFilesystemIO>( Base::Filesystem::Path() );
}

// test deafult parameter's value
template<>
template<>
void testObj::test<6>(void)
{
  const Base::Filesystem::Path out=createTempDirectory();
  ensure("not created", exists(out) );
  ensure("dir not created", is_directory(out) );
  remove(out);
}

} // namespace tut
