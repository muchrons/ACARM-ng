/*
 * createFile.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/createFile.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons;
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
      createFile(p);    // should throw
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
    tut::ensure("file already exists", exists(p)==false );
    SharedPtrNotNULL<std::fstream> out=createFile(p);
    tut::ensure("file not created", exists(p) );
    remove(p);
    tut::ensure("NULL pointer", out.get()!=NULL );
    tut::ensure("stream is not opened", out->is_open() );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/createFile");
} // unnamed namespace


namespace tut
{

// test throw when unable to create file
template<>
template<>
void testObj::test<1>(void)
{
  ensureThrow<ExceptionFilesystemIO>("/i_cannot_create_this_file");
}

// test exception when file is to be created inside dir with links in path
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/dirSymlink/other_dir/newFile.txt");
}

// try creating some file in current directory
template<>
template<>
void testObj::test<3>(void)
{
  ensureOk("newFile.txt");
}

// test creating file within some dir
template<>
template<>
void testObj::test<4>(void)
{
  ensureOk("testdata/dir/newFile.txt");
}

// test empty path
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow<ExceptionFilesystemIO>( path() );
}

} // namespace tut
