/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/Config.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/Config");
} // unnamed namespace


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
  // TODO
}

// 
template<>
template<>
void testObj::test<3>(void)
{
  // TODO
}

// 
template<>
template<>
void testObj::test<4>(void)
{
  // TODO
}

// 
template<>
template<>
void testObj::test<5>(void)
{
  // TODO
}

} // namespace tut
