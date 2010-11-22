/*
 * openElement.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Filesystem/openElement.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons::Filesystem;

namespace
{

bool isSaneTrue(const path &)
{
  return true;
}
bool isSaneFalse(const path &)
{
  return false;
}

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Filesystem/openElement");
} // unnamed namespace


namespace tut
{

// test if opening file that passes sanity checks passes
template<>
template<>
void testObj::test<1>(void)
{
  ensure("file not opened", openElement("testdata/file", Mode::READ, isSaneTrue).get()!=NULL );
}

// test if call throws on non-passing sanity check
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    openElement("testdata/file", Mode::READ, isSaneFalse);
    fail("call didn't throw on error");
  }
  catch(const ExceptionFilesystemIO &)
  {
    // this is expected
  }
}

// test if call throws on file-opening error
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    openElement("/i/do/not/exist", Mode::READWRITE, isSaneTrue);
    fail("call didn't throw on error");
  }
  catch(const ExceptionFilesystemIO &)
  {
    // this is expected
  }
}

} // namespace tut
