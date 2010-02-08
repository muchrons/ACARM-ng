/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

// TODO

#include "Filter/OneToMany/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Filter::OneToMany;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  Filter::BackendProxy::ChangedNodes changed_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/OneToMany/Strategy");
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
