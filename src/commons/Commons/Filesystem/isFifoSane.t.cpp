/*
 * isFifoSane.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Commons/Filesystem/isFifoSane.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  ~TestClass(void)
  {
    if( !tf_.empty() )
      remove(tf_);
  }

  void makeFifo(const path &p)
  {
    remove(p);
    tut::ensure_equals("unable to create fifo", mkfifo( p.string().c_str(), 0600 ), 0);
    tf_=p;
  }

  path tf_;     // TestFifo
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/isFifoSane");
} // unnamed namespace


namespace tut
{

// test if some fifo is sane
template<>
template<>
void testObj::test<1>(void)
{
  makeFifo("some_fifo");
  ensure("normal fifo not sane", isFifoSane("some_fifo") );
}

// test if check for non-exisitng file fails
template<>
template<>
void testObj::test<2>(void)
{
  ensure("non-existing fifo marked sane", isFifoSane("some/non/existing/file")==false );
}

// test if link to fifo is not sane
template<>
template<>
void testObj::test<3>(void)
{
  makeFifo("testdata/fifo");
  ensure("link marked sane", isFifoSane("testdata/fifoSymlink")==false );
}

// test if dongling symlink throws
template<>
template<>
void testObj::test<4>(void)
{
  ensure("dongling symlink marked sane", isFifoSane("testdata/donglingSymlink")==false );
}

// test if throws on dir
template<>
template<>
void testObj::test<5>(void)
{
  ensure("dir marked sane", isFifoSane("testdata/dir")==false );
}

// test if links in path are mathed as well
template<>
template<>
void testObj::test<6>(void)
{
  makeFifo("testdata/dirSymlink/other_dir/some_fifo");
  ensure("link inside path marked sane", isFifoSane("testdata/dirSymlink/other_dir/some_fifo")==false );
}

// test some relative file path
template<>
template<>
void testObj::test<7>(void)
{
  makeFifo("testdata/fifo");
  ensure("relative fifo not marked sane", isFifoSane("testdata/fifo") );
}

} // namespace tut
