/*
 * Builder.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <dlfcn.h>

#include "System/Plugins/Builder.hpp"

using namespace System::Plugins;

namespace
{
struct TestClass
{
  Builder b_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Plugins/Builder");
} // unnamed namespace


namespace tut
{

// test opening handle to some shared object
template<>
template<>
void testObj::test<1>(void)
{
  DynamicObject h=b_.open("testdata/sharedobj.so");
}

// test opening non-existing object
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    b_.open("/this/file/does/not/exist");
    fail("opening non-existign file didn't failed");
  }
  catch(const ExceptionCannotOpenSharedObject &)
  {
    // this is expected
  }
}

// test non-default flags settings
template<>
template<>
void testObj::test<3>(void)
{
  Builder b(true, false);
  DynamicObject h=b.open("testdata/sharedobj.so");
}

} // namespace tut
