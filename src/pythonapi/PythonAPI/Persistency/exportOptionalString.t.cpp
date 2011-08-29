/*
 * exportOptionalString.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "PythonAPI/Persistency/OptionalString.hpp"

using namespace std;
using namespace boost::python;
using namespace PythonAPI::Persistency;

namespace
{
struct TestClass
{
  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportOptionalString");
} // unnamed namespace

namespace tut
{

// test getting some random value
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("ret=tmp.get(\"metaalert.alert.name\")");
  const OptionalString ret( env_.var<OptionalString>("ret") );
  ensure("got NULL value", ret.get()!=NULL);
  ensure_equals("invalid value returned", ret.get(), string("some alert"));
}

// test getting some NULL value
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("ret=tmp.get(\"metaalert.alert.source.0.name\")");
  const OptionalString ret( env_.var<OptionalString>("ret") );
  ensure("got non-NULL value", ret.get()==NULL);
}

} // namespace tut
