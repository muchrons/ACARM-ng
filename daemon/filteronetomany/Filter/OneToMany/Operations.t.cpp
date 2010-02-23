/*
 * Operations.t.cpp
 *
 */
#include <tut.h>

#include "Filter/OneToMany/Operations.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Persistency;
using namespace Filter::OneToMany;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/OneToMany/Operations");
} // unnamed namespace

// TODO

namespace tut
{

//
template<>
template<>
void testObj::test<1>(void)
{
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

} // namespace tut
