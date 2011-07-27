/*
 * exportCertainty.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Environment.hpp"

using namespace std;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    env_.importModule("persistency");
    //env_.run("from persistency import *");

    env_.run("tmp=persistency.Timestamp(1234)");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportTimestamp");
} // unnamed namespace

namespace tut
{

// test for get() member
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.get()");
  const size_t ret=env_.var<size_t>("ret");
  ensure_equals("invalid value returned", ret, 1234);
}

// test for string() member
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.str()");
  const string ret=env_.var<string>("ret");
  ensure_equals("invalid value returned", ret, "1970.01.01 00:20:34");
}

} // namespace tut
