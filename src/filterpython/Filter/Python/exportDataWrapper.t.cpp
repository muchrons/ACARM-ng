/*
 * exportDataWrapper.t.cpp
 *
 */
#include <tut.h>
#include <boost/shared_ptr.hpp>

#include "PythonAPI/Python.hpp"
#include "PythonAPI/Environment.hpp"
#include "Filter/Python/DataWrapper.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::Python;
using namespace TestHelpers::Persistency;


namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void)
  {
    env_.importModule("filterapi");
    //env_.run("from persistency import *");
  }

  PythonAPI::Environment env_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/exportDataWrapper");
} // unnamed namespace

namespace tut
{

// smoke test
template<>
template<>
void testObj::test<1>(void)
{
  env_.run("from filterapi import DataWrapper");
}

// try making an instance
template<>
template<>
void testObj::test<2>(void)
{
  env_.run("tmp=DataWrapper()");
  DataPtr ptr=env_.var<DataPtr>("tmp");
  ensure("pointer is NULL", ptr.get()!=NULL);
}

} // namespace tut
