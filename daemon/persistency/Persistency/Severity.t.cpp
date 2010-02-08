/*
 * Severity.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Severity.hpp"

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

typedef TestClass TestClass;
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

// check NOTICE
template<>
template<>
void testObj::test<3>(void)
{
  check(SeverityLevel::NOTICE, "notice");
}

// check warning
template<>
template<>
void testObj::test<4>(void)
{
  check(SeverityLevel::WARNING, "warning");
}

// check problem
template<>
template<>
void testObj::test<5>(void)
{
  check(SeverityLevel::PROBLEM, "problem");
}

// check error
template<>
template<>
void testObj::test<6>(void)
{
  check(SeverityLevel::ERROR, "error");
}

// check critical
template<>
template<>
void testObj::test<7>(void)
{
  check(SeverityLevel::CRITICAL, "critical");
}

// check comparison
template<>
template<>
void testObj::test<8>(void)
{
  const Severity s1(SeverityLevel::INFO);
  const Severity s2(SeverityLevel::INFO);
  ensure("comaprison failed for the same elements", s1==s2);
}

// check negative comparison
template<>
template<>
void testObj::test<9>(void)
{
  const Severity s1(SeverityLevel::DEBUG);
  const Severity s2(SeverityLevel::INFO);
  ensure("comaprison didn't failed for different elements", s1!=s2);
}

// check comparison for different elements
template<>
template<>
void testObj::test<10>(void)
{
  const Severity s1(SeverityLevel::INFO);
  const Severity s2(SeverityLevel::WARNING);
  ensure("comaprison didn't failed for different elements", !(s1==s2) );
}

// check negative comparison
template<>
template<>
void testObj::test<11>(void)
{
  const Severity s1(SeverityLevel::INFO);
  const Severity s2(SeverityLevel::INFO);
  ensure("comaprison didn't failed for different elements", !(s1!=s2) );
}

} // namespace tut
