/*
 * heartbeats.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigConsts/heartbeats.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/heartbeats");
} // unnamed namespace


namespace tut
{

// test maximum number of lost heartbeats
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid maximum heartbeat", maxLostHeartbeats, 3);
}

} // namespace tut
