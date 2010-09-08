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

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

} // namespace tut
