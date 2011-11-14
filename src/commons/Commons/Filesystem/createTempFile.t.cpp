/*
 * createTempFile.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/createTempFile.hpp"
#include "Commons/Filesystem/isFileSane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Base::Filesystem;
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
      createTempFile(p);    // should throw
      tut::fail("call didn't throw on error");
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }

  void ensureOk(const path &p) const
  {
    TempFile out=createTempFile(p);
    tut::ensure("not created", exists(out.second) );
    tut::ensure("file not created", isFileSane(out.second) );
    remove(out.second);
    tut::ensure("NULL pointer", out.first.get()!=NULL );
    tut::ensure("stream is not opened", out.first->is_open() );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/createTempFile");
} // unnamed namespace


namespace tut
{

// test throw when unable to create file
template<>
template<>
void testObj::test<1>(void)
{
  ensureThrow<ExceptionFilesystemIO>("/");
}

// test exception when file is to be created inside dir with links in path
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/dirSymlink/other_dir/");
}

// try creating some file in current directory
template<>
template<>
void testObj::test<3>(void)
{
  ensureOk(".");
}

// test creating file within some dir
template<>
template<>
void testObj::test<4>(void)
{
  ensureOk("testdata/dir/");
}

// test default argument
template<>
template<>
void testObj::test<5>(void)
{
  TempFile out=createTempFile();
  tut::ensure("not created", exists(out.second) );
  tut::ensure("file not created", isFileSane(out.second) );
  remove(out.second);
  tut::ensure("NULL pointer", out.first.get()!=NULL );
  tut::ensure("stream is not opened", out.first->is_open() );
}

// test empty path
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow<ExceptionFilesystemIO>( path() );
}

// test throw on non existing directory
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/nonexistingdirectory/");
}

} // namespace tut
