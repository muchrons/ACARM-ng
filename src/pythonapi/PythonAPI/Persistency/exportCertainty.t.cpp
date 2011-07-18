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

    env_.run("c=persistency.Certainty(0.42)");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportCertainty");
} // unnamed namespace

namespace tut
{

// test for get() member
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=c.get()");
  const double ret=env_.var<double>("ret");
  ensure_equals("invalid value returned", ret, 0.42);
}

} // namespace tut
