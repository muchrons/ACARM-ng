/*
 * Handle.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <dlfcn.h>

#include "System/Plugins/Handle.hpp"
#include "System/Plugins/TestBase.t.hpp"

using namespace System::Plugins;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Plugins/Handle");
} // unnamed namespace


namespace tut
{

// test NULL pointer
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    Handle h(NULL);
    fail("c-tor didn't failed for NULL");
  }
  catch(const System::ExceptionPointerIsNULL &)
  {
    // this is expected
  }
}

// test non-NULL pointer
template<>
template<>
void testObj::test<2>(void)
{
  Handle h( openShared() );
}

// test accessor method
template<>
template<>
void testObj::test<3>(void)
{
  Handle h( openShared() );
  ensure("pointer is NULL", h.get()!=NULL );    // compile-check, in fact
}

// test pointer typedef presence (compile-test)
template<>
template<>
void testObj::test<4>(void)
{
  HandlePtrNN h( new Handle( openShared() ) );
}

} // namespace tut
