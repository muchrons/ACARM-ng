/*
 * Environment.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace PythonAPI;
namespace py=boost::python;

namespace
{
// define some test class
struct TestStruct
{
  int getAnswer(void) const
  {
    return 42;
  }
}; // struct TestStruct
// make it python-visible
BOOST_PYTHON_MODULE(environment_test_module)
{
  py::class_<TestStruct>("TestStruct")
    .def("getAnswer", &TestStruct::getAnswer)
  ;
}
// import this module to the python
PythonAPI::Environment::StaticImporter g_importDummy("environment_test_module", initenvironment_test_module);
//PythonAPI::Environment::StaticImporter g_importDummy("environment_test_module", PyInit_environment_test_module);


struct TestClass
{
  Environment e_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Environment");
} // unnamed namespace

namespace tut
{

// test multiple instances creation
template<>
template<>
void testObj::test<1>(void)
{
  Environment e;
}

// test running some script
template<>
template<>
void testObj::test<2>(void)
{
  e_.run("c=1+2");
}

// test running some script that throws exception
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    e_.run("c='1'+1");
    fail("exception has not been forwarded from script");
  }
  catch(const ExceptionFromScript &)
  {
    // this is exepcted
  }
}

// test getting some variable as a copy
template<>
template<>
void testObj::test<4>(void)
{
  e_.run("a=42");
  const int a=e_.var<int>("a");
  ensure_equals("invalid value", a, 42);
}

// test importing standard module
template<>
template<>
void testObj::test<5>(void)
{
  e_.importModule("sys");
  e_.run("tmp=sys.version");
  // check
  const char *str=e_.var<const char*>("tmp");
  ensure("got NULL pointer", str!=NULL);
  ensure("string too short", std::string(str).length()>5 );
}

// test importing own module
template<>
template<>
void testObj::test<6>(void)
{
  e_.importModule("environment_test_module");
  e_.run("tmp=environment_test_module.TestStruct()");
  e_.run("tmp=tmp.getAnswer()");
  // check
  const int answer=e_.var<int>("tmp");
  ensure_equals("invalid answer set", answer, 42);
}

// NOTE: initializing of module seems to always succeed, but sometime has "funny" sideeffects... :/
//       due to this issues it won't be tested here.

// test requesting export as invalid value
template<>
template<>
void testObj::test<7>(void)
{
  e_.run("fp=4.2");
  try
  {
    e_.var<int>("fp");  // should throw, since convertion is not possible
    fail("no exception risen by invalid convertion");
  }
  catch(const ExceptionFromScript &)
  {
    // this is expected
  }
}

// test returning string
template<>
template<>
void testObj::test<8>(void)
{
  e_.run("tmp=\"alice has aids\"");
  // check
  const string answer=e_.var<string>("tmp");
  ensure_equals("invalid answer set", answer, "alice has aids");
}

} // namespace tut
