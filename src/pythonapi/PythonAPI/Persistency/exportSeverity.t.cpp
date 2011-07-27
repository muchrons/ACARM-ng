/*
 * exportSeverity.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/SeverityLevel.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace ::Persistency;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    env_.importModule("persistency");
    //env_.run("from persistency import *");

    env_.run("tmp=persistency.SeverityLevel(persistency.SeverityLevel.EType.INFO)");
    env_.run("tmp=persistency.Severity(tmp)");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportSeverity");
} // unnamed namespace

namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.getName()");
  const char *ret=env_.var<const char*>("ret");
  ensure_equals("invalid value returned", ret, string("info") );
}

// test getting level
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.getLevel()");
  const SeverityLevel ret=env_.var<SeverityLevel>("ret");
  ensure("invalid value returned", ret==SeverityLevel::INFO);
}

} // namespace tut
