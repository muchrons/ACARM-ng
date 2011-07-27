/*
 * exportSeverityLevel.t.cpp
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

    env_.run("tmp=persistency.SeverityLevel(persistency.SeverityLevel.EType.INFO)");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportSeverityLevel");
} // unnamed namespace

namespace tut
{

// test for get() member
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.toInt()");
  const int ret=env_.var<int>("ret");
  ensure_equals("invalid value returned", ret, 1);
}

// test for DEBUG severity existance
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=persistency.SeverityLevel(persistency.SeverityLevel.EType.DEBUG)");
}

// test for LOWseverity existance
template<>
template<>
void testObj::test<3>(void)
{
  env_.run("ret=persistency.SeverityLevel(persistency.SeverityLevel.EType.LOW)");
}

// test for MEDIUM severity existance
template<>
template<>
void testObj::test<4>(void)
{
  env_.run("ret=persistency.SeverityLevel(persistency.SeverityLevel.EType.MEDIUM)");
}

// test for HIGH severity existance
template<>
template<>
void testObj::test<5>(void)
{
  env_.run("ret=persistency.SeverityLevel(persistency.SeverityLevel.EType.HIGH)");
}

} // namespace tut
