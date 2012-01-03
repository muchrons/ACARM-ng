/*
 * filters.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigConsts/filters.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/filters");
} // unnamed namespace


namespace tut
{

// test default input type
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid timeout for too old entries", maxFilterQueueTimeoutForTooOld, 66);
}

} // namespace tut
