/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "RFCIO/XML/Reader.hpp"

using namespace std;
using namespace RFCIO::XML;

namespace
{

struct TestClass
{
  std::stringstream ss_;
  Reader            r_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("RFCIO/XLM/Reader");
} // unnamed namespace


namespace tut
{

// test reading from invalid stream
template<>
template<>
void testObj::test<1>(void)
{
/* NOTE: this test is commented-out since libxml2 prints debug info
         directly on the screen...
  ss_<<"hello junks";
  try
  {
    r_.read(ss_);
    fail("read() didn't throw on invalid XML");
  }
  catch(const Exception &)
  {
    // this is expected
  }
*/
}

// test reading valid XML
template<>
template<>
void testObj::test<2>(void)
{
  ss_<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<endl;
  ss_<<"<hello_test/>"<<endl;
  r_.read(ss_);
}

} // namespace tut
