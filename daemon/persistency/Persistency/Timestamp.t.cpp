/*
 * Timestamp.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Timestamp.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Timestamp");
} // unnamed namespace


namespace tut
{

// check default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  Timestamp ts;
}

// check (in)equality operators
template<>
template<>
void testObj::test<2>(void)
{
  const Timestamp ts1=boost::posix_time::time_from_string("2010-11-12");
  const Timestamp ts2=boost::posix_time::time_from_string("2009-10-11");
  TestHelpers::checkEquality(ts1, ts2);
}

} // namespace tut
