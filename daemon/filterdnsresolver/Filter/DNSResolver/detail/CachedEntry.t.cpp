/*
 * CachedEntry.t.cpp
 *
 */
#include <tut.h>

#include "Filter/DNSResolver/detail/CachedEntry.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::DNSResolver::detail;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  CachedEntry::IP ip(const char *ip) const
  {
    return CachedEntry::IP::from_string(ip);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/detail/CachedEntry");
} // unnamed namespace


namespace tut
{

// test creating object
template<>
template<>
void testObj::test<1>(void)
{
  const CachedEntry ce( ip("1.2.3.4"), "alice.has.a.cat", 21);
  ensure("invalid IP", ce.ip_==ip("1.2.3.4") );
  ensure("invalid name", ce.name_=="alice.has.a.cat" );
  ensure("invalid timeout", time(NULL)+21-ce.time_<20 );
}

// test SWO
template<>
template<>
void testObj::test<2>(void)
{
  const CachedEntrySWO swo=CachedEntrySWO();
  const CachedEntry    ce1( ip("2.2.3.4"),  "alice.has.a.cat", 21);
  const CachedEntry    ce2( ip("2.1.30.4"), "alice.has.a.cat", 21);
  const CachedEntry    ce3( ip("3.2.3.4"),  "alice.has.a.cat", 21);

  ensure("comaprison 1 failed",  swo(ce2, ce1) );
  ensure("comaprison 2 failed",  swo(ce2, ce3) );
  ensure("comaprison 3 failed",  swo(ce1, ce3) );

  ensure("comaprison 4 failed", !swo(ce1, ce2) );
  ensure("comaprison 5 failed", !swo(ce3, ce2) );
  ensure("comaprison 6 failed", !swo(ce3, ce1) );

  ensure("comaprison 7 failed", !swo(ce1, ce1) );
}

} // namespace tut
