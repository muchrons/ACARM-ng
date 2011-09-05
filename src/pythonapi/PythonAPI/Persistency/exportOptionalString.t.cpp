/*
 * exportOptionalString.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Environment.hpp"
#include "PythonAPI/Persistency/OptionalString.hpp"

using namespace std;
using namespace PythonAPI::Persistency;

namespace
{
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

factory tf("PythonAPI/Persistency/exportOptionalString");
} // unnamed namespace

namespace tut
{

// test creating some instance
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("tmp=persistency.OptionalString(\"test string\")");
  ensure_equals("invalid value", env_.var<OptionalString>("tmp").get(), string("test string"));
}

// test creating from None
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("tmp=persistency.OptionalString(None)");
  ensure("string not NULL", env_.var<OptionalString>("tmp").get()==NULL);
}

} // namespace tut
