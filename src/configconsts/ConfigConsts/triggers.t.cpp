/*
 * triggers.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "ConfigConsts/triggers.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/triggers");
} // unnamed namespace


namespace tut
{

// test truncating queue size
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid truncating queue size", waitingTruncQueueSize, 1024);
}

} // namespace tut
