/*
 * PersistencyCleanup.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Core/PersistencyCleanup.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/PersistencyCleanup");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  PersistencyCleanup pc;
}

// test manual call to cleanup
template<>
template<>
void testObj::test<2>(void)
{
  PersistencyCleanup pc;
  pc.cleanup();
}

} // namespace tut
