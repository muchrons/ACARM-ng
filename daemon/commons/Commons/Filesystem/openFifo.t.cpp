/*
 * openFifo.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Commons/Filesystem/openFifo.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
  TestClass(void):
    fifo_("some_fifo")
  {
    remove(fifo_);
    tut::ensure_equals("unable to create fifo", mkfifo( fifo_.string().c_str(), 0600 ), 0);
  }
  ~TestClass(void)
  {
    remove(fifo_);
  }

  const path fifo_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/openFifo");
} // unnamed namespace


namespace tut
{

// test if opening fifo that passes sanity checks passes
template<>
template<>
void testObj::test<1>(void)
{
  ensure("fifo not opened", openFifo(fifo_).get()!=NULL );
}

// test if call throws on non-passing sanity check
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    openFifo("testdata/file");
    fail("call didn't throw on error");
  }
  catch(const ExceptionFilesystemIO &)
  {
    // this is expected
  }
}

// test passing R/W mode explicit
template<>
template<>
void testObj::test<3>(void)
{
  ensure("fifo not opened", openFifo(fifo_, Mode::READ).get()!=NULL );
}

} // namespace tut
