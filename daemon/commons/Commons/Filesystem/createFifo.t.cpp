/*
 * createFifo.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/createFifo.hpp"

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
      createFifo(p);    // should throw
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
    tut::ensure("queue already exists", exists(p)==false );
    SharedPtrNotNULL<std::fstream> out=createFifo(p);
    tut::ensure("queue not created", exists(p) );
    remove(p);
    tut::ensure("NULL pointer", out.get()!=NULL );
    tut::ensure("stream is not opened", out->is_open() );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/createFifo");
} // unnamed namespace


namespace tut
{

// test throw when unable to create fifo
template<>
template<>
void testObj::test<1>(void)
{
  ensureThrow<ExceptionFilesystemIO>("/i/cannot/create/this/fifo/queue");
}

// test exception when fifo is to be created inside dir with links in path
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow<ExceptionFilesystemIO>("testdata/dirSymlink/other_dir/new_fifo");
}

// try creating some fifo in current directory
template<>
template<>
void testObj::test<3>(void)
{
  ensureOk("new_fifo");
}

// test creating fifo within some dir
template<>
template<>
void testObj::test<4>(void)
{
  ensureOk("testdata/dir/fifo_queue");
}

// test empty path
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow<ExceptionFilesystemIO>( path() );
}

} // namespace tut
