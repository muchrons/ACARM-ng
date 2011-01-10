/*
 * Main.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <boost/thread.hpp>

#include "Core/Main.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/Main");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Main m;
}

// test stopping request and joining
template<>
template<>
void testObj::test<2>(void)
{
  Main m;
  m.stop();
  m.waitUntilDone();
}

} // namespace tut
