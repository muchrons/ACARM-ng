/*
 * exports.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Environment.hpp"

namespace
{
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
