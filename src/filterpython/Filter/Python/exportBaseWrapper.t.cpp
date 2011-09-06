/*
 * exportBaseWrapper.t.cpp
 *
 */
#include <tut.h>
#include <boost/shared_ptr.hpp>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/BaseWrapper.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::Python;
using namespace TestHelpers::Persistency;


namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void)
  {
    env_.importModule("filterapi");
    //env_.run("from persistency import *");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/exportBaseWrapper");
} // unnamed namespace

namespace tut
{

// smoke test
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("from filterapi import BaseWrapper");
}

/*
// try making an instance
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("tmp=BaseWrapper()");
  BasePtr ptr=env_.var<BasePtr>("tmp");
  ensure("pointer is NULL", ptr.get()!=NULL);
  try
  {
    ptr->filter(makeNewLeaf());
    fail("call didn't throw on non-existing override method");
  }
  catch(const ExceptionNoImplementation&)
  {
    // this is expected
  }
}
*/

} // namespace tut
