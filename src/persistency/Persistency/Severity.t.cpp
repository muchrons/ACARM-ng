/*
 * Severity.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Severity.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
{
  void check(const SeverityLevel sl, const char *name) const
  {
    assert(name!=NULL);
    const Severity ti(sl);
    tut::ensure_equals("invalid severity level",
                       ti.getLevel().toInt(), sl.toInt() );
    tut::ensure_equals("invalid severity name",
                       ti.getName(),          string(name) );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Severity");
} // unnamed namespace


namespace tut
{

// check level "DEBUG"
template<>
template<>
void testObj::test<1>(void)
{
  check(SeverityLevel::DEBUG, "debug");
}

// check INFO
template<>
template<>
void testObj::test<2>(void)
{
  check(SeverityLevel::INFO, "info");
}

// check low
template<>
template<>
void testObj::test<3>(void)
{
  check(SeverityLevel::LOW, "low");
}

// check medium
template<>
template<>
void testObj::test<4>(void)
{
  check(SeverityLevel::MEDIUM, "medium");
}

// check high
template<>
template<>
void testObj::test<5>(void)
{
  check(SeverityLevel::HIGH, "high");
}

// check comparison
template<>
template<>
void testObj::test<8>(void)
{
  const Severity s1(SeverityLevel::INFO);
  const Severity s2(SeverityLevel::INFO);
  const Severity s3(SeverityLevel::DEBUG);
  TestHelpers::checkEquality(s1, s2, s3);
}

// check negative comparison
template<>
template<>
void testObj::test<9>(void)
{
  const Severity s1(SeverityLevel::DEBUG);
  const Severity s2(SeverityLevel::INFO);
  TestHelpers::checkEquality(s1, s2);
}

// check comparison for different elements
template<>
template<>
void testObj::test<10>(void)
{
  const Severity s1(SeverityLevel::INFO);
  const Severity s2(SeverityLevel::LOW);
  TestHelpers::checkEquality(s1, s2);
}

} // namespace tut
