/*
 * Builder.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <dlfcn.h>
#include <boost/filesystem.hpp>

#include "System/Plugins/Builder.hpp"
#include "System/Plugins/TestBase.t.hpp"
#include "System/ignore.hpp"

using namespace System::Plugins;
namespace fs=boost::filesystem;

namespace
{
struct TestClass: private TestBase
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
  DynamicObject h=b_.open("./sharedobj.so");
  System::ignore(h);
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
  Builder       b(false, false);
  DynamicObject h=b.open("./sharedobj.so");
  System::ignore(h);
}

// test if opening works when just file name is given
template<>
template<>
void testObj::test<4>(void)
{
  b_.open("sharedobj.so");  // should not throw
}

// test opening via full path
template<>
template<>
void testObj::test<5>(void)
{
  const fs::path full=fs::system_complete("sharedobj.so");
  b_.open(full);            // should not throw
}

} // namespace tut
