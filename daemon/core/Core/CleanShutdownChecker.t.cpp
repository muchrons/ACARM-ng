/*
 * CleanShutdownChecker.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Core/CleanShutdownChecker.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/CleanShutdownChecker");
} // unnamed namespace


namespace tut
{

// test not-confirmed exit - smoke test
template<>
template<>
void testObj::test<1>(void)
{
  CleanShutdownChecker csc;
}

// test confirmed exit - smoke test
template<>
template<>
void testObj::test<2>(void)
{
  CleanShutdownChecker csc;
  csc.confirmCleanShutdown();
}

} // namespace tut
