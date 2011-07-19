/*
 * exportMD5Sum.t.cpp
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

    env_.run("tmp=persistency.MD5Sum.createFromString(\"78b9861f74e15d7d0f077ba22421b8e4\")");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportMD5Sum");
} // unnamed namespace

namespace tut
{

// test for get() member
template<>
template<>
void testObj::test<1>(void)
{
  const string evil("78b9861f74e15d7d0f077ba22421b8e4");
  env_.run("ret=tmp.get()");
  const char *ret=env_.var<const char*>("ret");
  ensure_equals("invalid value returned", ret, evil);
}

} // namespace tut
