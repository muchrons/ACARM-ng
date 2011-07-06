/*
 * Environment.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "PythonAPI/Environment.hpp"

using namespace PythonAPI;


extern "C"
{
static PyObject* dummyModuleInit(void)
{
  return NULL;
} // dummyModuleInit
} // extern "C"

namespace
{
// following line is smoke-test
PythonAPI::Environment::StaticImporter g_importDummy("myDummyModule", dummyModuleInit);

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

// NOTE: initializing of module seems to always succeed... :/

} // namespace tut
