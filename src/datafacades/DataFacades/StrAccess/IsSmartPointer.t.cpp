/*
 * IsSmartPointer.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/StrAccess/IsSmartPointer.hpp"

using namespace DataFacades::StrAccess;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/IsSmartPointer");
} // unnamed namespace


namespace tut
{

// test non-smart-pointer
template<>
template<>
void testObj::test<1>(void)
{
  ensure("invalid result", IsSmartPointer<int>::value==false);
}

// test raw pointer
template<>
template<>
void testObj::test<2>(void)
{
  ensure("invalid result", IsSmartPointer<int*>::value==false);
}

// test boost::shared_ptr
template<>
template<>
void testObj::test<3>(void)
{
  ensure("invalid result", IsSmartPointer< boost::shared_ptr<int> >::value);
}

// test System::SharedPtrNotNULL
template<>
template<>
void testObj::test<4>(void)
{
  ensure("invalid result", IsSmartPointer< System::SharedPtrNotNULL<int> >::value);
}

// test Commons::SharedPtrNotNULL
template<>
template<>
void testObj::test<5>(void)
{
  ensure("invalid result", IsSmartPointer< Commons::SharedPtrNotNULL<int> >::value);
}

// test boost::scoped_ptr
template<>
template<>
void testObj::test<6>(void)
{
  ensure("invalid result", IsSmartPointer< boost::scoped_ptr<int> >::value);
}

} // namespace tut
