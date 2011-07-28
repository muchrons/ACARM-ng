/*
 * exportReferenceURL.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/ReferenceURL.hpp"
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

    env_.run("tmp=persistency.ReferenceURL( "
                    "persistency.ReferenceURL.Name(\"some name\"),"
                    "persistency.ReferenceURL.URL(\"http://url\") )");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportReferenceURL");
} // unnamed namespace

namespace tut
{

// test for getting name
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.getName()");
  const ReferenceURL::Name &ret=env_.var<const ReferenceURL::Name&>("ret");
  ensure_equals("invalid value returned", ret.get(), string("some name") );
}

// test for getting url
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.getURL()");
  const ReferenceURL::URL &ret=env_.var<const ReferenceURL::URL&>("ret");
  ensure_equals("invalid value returned", ret.get(), string("http://url") );
}

} // namespace tut
