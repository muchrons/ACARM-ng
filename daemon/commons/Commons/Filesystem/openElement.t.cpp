/*
 * openElement.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <sys/stat.h>

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

} // namespace tut
