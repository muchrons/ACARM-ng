/*
 * IsPointer.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/StrAccess/IsPointer.hpp"

using namespace DataFacades::StrAccess;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/IsPointer");
} // unnamed namespace


namespace tut
{

// test non-pointer
template<>
template<>
void testObj::test<1>(void)
{
  ensure("invalid result", IsPointer<int>::value==false);
}

// test pointer
template<>
template<>
void testObj::test<2>(void)
{
  ensure("invalid result", IsPointer<int*>::value);
}

// test const pointer
template<>
template<>
void testObj::test<3>(void)
{
  ensure("invalid result", IsPointer<const int*>::value);
}

// test volatile pointer
template<>
template<>
void testObj::test<4>(void)
{
  ensure("invalid result", IsPointer<volatile int*>::value);
}

// test const volatile pointer
template<>
template<>
void testObj::test<5>(void)
{
  ensure("invalid result", IsPointer<const volatile int*>::value);
}

} // namespace tut
