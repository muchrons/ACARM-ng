/*
 * createDirectory.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/createDirectory.hpp"

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
      createDirectory(p);   // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }

  void ensureOk(const path &p) const
  {
    remove(p);
    tut::ensure("dir already exists", exists(p)==false );
    createDirectory(p);
    tut::ensure("dir not created", exists(p) );
    remove(p);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/createDirectory");
} // unnamed namespace


namespace tut
{

// test throw when unable to create directory
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
  ensureThrow<ExceptionFilesystemIO>("testdata/dirSymlink/other_dir/new_dir");
}

// try creating some dir in current directory
template<>
template<>
void testObj::test<3>(void)
{
  ensureOk("new_directory");
}

// test creating directory within some dir
template<>
template<>
void testObj::test<4>(void)
{
  ensureOk("testdata/dir/new_dir");
}

} // namespace tut
