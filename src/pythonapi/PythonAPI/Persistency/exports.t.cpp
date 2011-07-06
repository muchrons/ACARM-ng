/*
 * exports.t.cpp
 *
 */
#include <tut.h>
#include <boost/scoped_ptr.hpp>

#include "PythonAPI/Persistency/exports.hpp"
#include "PythonAPI/Environment.hpp"

using namespace std;
using namespace boost::python;

namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("persistency", PyInit_persistency);

struct TestClass
{
  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exports");
} // unnamed namespace

namespace tut
{

// test for Certainty class' presence
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("c=persistency.Certainty(0.42)\n");  // smoke test - in case of error call will throw
}

} // namespace tut
