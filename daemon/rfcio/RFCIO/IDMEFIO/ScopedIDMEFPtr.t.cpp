/*
 * ScopedIDMEFPtr.t.cpp
 *
 */
#include <tut.h>

#include "RFCIO/IDMEFIO/ScopedIDMEFPtr.hpp"

using namespace RFCIO::IDMEFIO;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;
factory tf("RFCIO/IDMEF/ScopedIDMEFPtr");
}

namespace tut
{

// test NULL pointer (smoke test)
template<>
template<>
void testObj::test<1>(void)
{
  ScopedIDMEFPtr ptr(NULL);
}

// test freeing some pointer (smoke test)
template<>
template<>
void testObj::test<2>(void)
{
  ScopedIDMEFPtr ptr( newIDMEF_Message_version("1.0", NULL) );
}

// test freeing some pointer, recursively (somek test)
template<>
template<>
void testObj::test<3>(void)
{
  ScopedIDMEFPtr ptr( newIDMEF_Message( newIDMEF_Message_version("1.0", NULL), NULL ) );
}

} // namespace tut
