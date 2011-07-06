/*
 * Environment.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "PythonAPI/Environment.hpp"

using namespace PythonAPI;

namespace
{
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

// test importing module with NULL initalizer
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    Environment::importModule("abc", NULL);
    fail("module importing didn't failed with NULL initializer");
  }
  catch(const Exception &)
  {
    // this is expected
  }
}


extern "C"
{
static PyObject* dummyModuleInit(void)
{
  return NULL;
} // dummyModuleInit
} // extern "C"
// test importing some module (may not exist)
template<>
template<>
void testObj::test<3>(void)
{
  Environment::importModule("abc", dummyModuleInit);
}

// test running some script
template<>
template<>
void testObj::test<4>(void)
{
  e_.run("c=1+2");
}

// test running some script that throws exception
template<>
template<>
void testObj::test<5>(void)
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
void testObj::test<6>(void)
{
  e_.run("a=42");
  const int a=e_.var<int>("a");
  ensure_equals("invalid value", a, 42);
}

// NOTE: initializing of module seems to always succeed... :/

} // namespace tut
