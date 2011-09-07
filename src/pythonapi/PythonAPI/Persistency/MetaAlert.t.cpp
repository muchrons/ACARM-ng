/*
* MetaAlert.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace PythonAPI::Persistency;
using namespace TestHelpers::Persistency;

namespace
{
struct TestClass: private TestStubs
{
  TestClass(void):
    node_( makeNewLeaf("1.2.3.4", NULL, false) ),
    ma_(node_)
  {
  }

  void testEqual(const char *path, const char *exp) const
  {
    OptionalString out=ma_.get(string(path));
    if(exp==NULL)
      tut::ensure("output string is not NULL", out.get()==NULL);
    else
      if(out.get()==NULL)
        tut::fail("unexpected NULL reponse");
      else
        tut::ensure_equals("invalid response", out.get(), string(exp));
  }

  ::Persistency::ConstGraphNodePtrNN node_;
  MetaAlert                          ma_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/MetaAlert");
} // unnamed namespace

namespace tut
{

// test gettting value of a sample field
template<>
template<>
void testObj::test<1>(void)
{
  testEqual("metaalert.alert.name", "some alert");
}

// test getting NULL value
template<>
template<>
void testObj::test<2>(void)
{
  testEqual("metaalert.alert.sources.0.name", NULL);
}

// test getting colleciton size
template<>
template<>
void testObj::test<3>(void)
{
  testEqual("metaalert.alert.sources.size", "1");
}

// test getting colleciton size, when not a colleciton item
template<>
template<>
void testObj::test<4>(void)
{
  testEqual("metaalert.alert.size", NULL);
}

// test getting colleciton element
template<>
template<>
void testObj::test<5>(void)
{
  testEqual("metaalert.alert.sources.0.ip", "1.2.3.4");
}

// test getting colleciton element out of range
template<>
template<>
void testObj::test<6>(void)
{
  testEqual("metaalert.alert.sources.2.ip", NULL);
}

} // namespace tut
