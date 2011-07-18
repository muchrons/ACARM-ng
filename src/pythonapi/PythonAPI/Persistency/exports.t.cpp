/*
 * exports.t.cpp
 *
 * note - these test cases are not typical unit tests. due to the nature of the
 * python it has to be handled 
 *
 */
#include <tut.h>
#include <boost/scoped_ptr.hpp>

#include "PythonAPI/Persistency/exports.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace PythonAPI::Persistency;

namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("persistency", initpersistency);
//PythonAPI::Environment::StaticImporter g_import("persistency", PyInit_persistency);

struct TestClass
{
  TestClass(void)
  {
    env_.importModule("persistency");
    //env_.run("from persistency import *");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exports");
} // unnamed namespace

namespace tut
{

// test for Certainty class' API
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("c=persistency.Certainty(0.42)");
  env_.run("ret=c.get()");
  const double ret=env_.var<double>("ret");
  ensure_equals("invalid value returned", ret, 0.42);
}

// test for Timestamp class' API
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("t=persistency.Timestamp(1234)");
  env_.run("ret=t.get()");
  const size_t ret=env_.var<size_t>("ret");
  ensure_equals("invalid value returned", ret, 1234);
}

} // namespace tut
