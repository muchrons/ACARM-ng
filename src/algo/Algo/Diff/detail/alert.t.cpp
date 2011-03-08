/*
 * alert.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/alert.hpp"

using namespace Persistency;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/alert");
} // unnamed namespace


namespace tut
{

// check if identical severities are the same
template<>
template<>
void testObj::test<1>(void)
{
}

// test slightly different alerts
template<>
template<>
void testObj::test<2>(void)
{
}

// test totally different alerts
template<>
template<>
void testObj::test<3>(void)
{
}

#warning TODO: write tests

} // namespace tut
