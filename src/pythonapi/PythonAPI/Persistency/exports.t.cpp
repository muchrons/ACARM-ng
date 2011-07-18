/*
 * exports.t.cpp
 *
 * NOTE: this is NOT test suit but import scheduler for classes used in tests.
 *
 */
#include <tut.h>

#include "PythonAPI/Persistency/exports.hpp"
#include "PythonAPI/Environment.hpp"

namespace
{
// it needs to be imported before initializing environment
PythonAPI::Environment::StaticImporter g_import("persistency", initpersistency);
//PythonAPI::Environment::StaticImporter g_import("persistency", PyInit_persistency);

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

// tets if namespace is present
template<>
template<>
void testObj::test<1>(void)
{
  env_.importModule("persistency");
  //env_.run("from persistency import *");
}

} // namespace tut
