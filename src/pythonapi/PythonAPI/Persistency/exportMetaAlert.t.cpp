/*
 * exportMetaAlert.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/MetaAlert.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace boost::python;
using namespace PythonAPI::Persistency;
using namespace TestHelpers::Persistency;

namespace
{
MetaAlert getTestData(void)
{
  return MetaAlert( makeNewLeaf("1.2.3.4", NULL, false) );
}

BOOST_PYTHON_MODULE(testpersistency)
{
  def("getTestData", getTestData);
}

// this needs to be executed before creating first environment
PythonAPI::Environment::StaticImporter g_import("testpersistency", inittestpersistency);

struct TestClass
{
  TestClass(void)
  {
    env_.importModule("testpersistency");
    //env_.run("from persistency import *");

    env_.run("tmp=testpersistency.getTestData()");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportMetaAlert");
} // unnamed namespace

namespace tut
{

// test getting some random value
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.get(\"metaalert.alert.name\")");
  const OptionalString ret=env_.var<OptionalString>("ret");
  ensure("got NULL value", ret.get()!=NULL);
  ensure_equals("invalid value returned", ret.get(), string("some alert"));
}

// test getting some NULL value
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.get(\"metaalert.alert.source.0.name\")");
  const OptionalString ret=env_.var<OptionalString>("ret");
  ensure("got non-NULL value", ret.get()==NULL);
}

} // namespace tut
