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

    //env_.run("tmp=persistency.Timestamp(1234)");
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
  // TODO
  /*
  env_.run("ret=tmp.get()");
  const size_t ret=env_.var<size_t>("ret");
  ensure_equals("invalid value returned", ret, 1234);
  */
}

} // namespace tut
