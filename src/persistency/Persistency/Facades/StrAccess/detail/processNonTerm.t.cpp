/*
 * processNonTerm.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/Facades/StrAccess/processNonTerm.hpp"

using namespace Persistency::Facades::StrAccess::detail;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/processNonTerm");
} // unnamed namespace


namespace tut
{

// test sample value
template<>
template<>
void testObj::test<1>(void)
{
}

// test raw pointer
template<>
template<>
void testObj::test<2>(void)
{
}

// test smart pointer
template<>
template<>
void testObj::test<3>(void)
{
}

// test collection
template<>
template<>
void testObj::test<4>(void)
{
}

} // namespace tut
