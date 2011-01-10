/*
 * openFile.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/openFile.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/openFile");
} // unnamed namespace


namespace tut
{

// test if opening file that passes sanity checks passes
template<>
template<>
void testObj::test<1>(void)
{
  ensure("file not opened", openFile("testdata/file").get()!=NULL );
}

// test if call throws on non-passing sanity check
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    openFile("testdata/dirSymlink/other_dir/some_file");
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
  ensure("file not opened", openFile("testdata/file", Mode::WRITE).get()!=NULL );
}

} // namespace tut
