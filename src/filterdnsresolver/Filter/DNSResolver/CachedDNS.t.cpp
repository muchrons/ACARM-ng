/*
 * CachedDNS.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "System/Timer.hpp"
#include "Filter/DNSResolver/CachedDNS.hpp"
#include "Filter/DNSResolver/TestIsValidLocalhost.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/delay.hpp"

using namespace std;
using namespace Filter::DNSResolver;
using TestHelpers::delay;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cache_(1)
  {
  }

  CachedEntry::IP ip(const char *ip) const
  {
    return CachedEntry::IP::from_string(ip);
  }

  CachedDNS cache_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/CachedDNS");
} // unnamed namespace


namespace tut
{

// test caching
template<>
template<>
void testObj::test<1>(void)
{
  const CachedEntry::Name &name=cache_[ ip("127.0.0.1") ].second;
  ensure("invalid name", isValidLocalhost( name.get() ) );
}

// test pruning
template<>
template<>
void testObj::test<2>(void)
{
  CachedDNS cache(0);       // we make this explicitly here, to set small timeout
  // create new entry
  cache_[ ip("127.0.0.1") ];
  // remove entry
  delay(2);
  cache_.prune();
  // add entry once more
  const CachedEntry::Name &name=cache_[ ip("127.0.0.1") ].second;
  ensure("invalid name", isValidLocalhost( name.get() ) );
}

// test resolving non-exisiting entry
template<>
template<>
void testObj::test<3>(void)
{
  const CachedDNS::Entry e=cache_[ ip("192.168.255.252") ];
  ensure("invalid name", !e.first );
  ensure_equals("invalid name", e.second.get(), string("") );
}

// test timing when reading from cache.
template<>
template<>
void testObj::test<4>(void)
{
  // measure first call, when DNS query is performed
  const System::Timer    t1;
  const CachedDNS::Entry e1      =cache_[ ip("127.0.0.1") ];
  const double           elapsed1=t1.elapsed();
  ensure("invalid name 1", e1.first );

  // meauser second call, when result is read from cache
  const System::Timer    t2;
  const CachedDNS::Entry e2      =cache_[ ip("127.0.0.1") ];
  const double           elapsed2=t2.elapsed();
  ensure("invalid name 2", e2.first );

  ensure("cached read took more time than query", elapsed1>elapsed2);
}

// test timing when reading query-miss (i.e. non-existing entry) from cache.
template<>
template<>
void testObj::test<5>(void)
{
  // measure first call, when DNS query is performed
  const System::Timer    t1;
  const CachedDNS::Entry e1      =cache_[ ip("192.168.255.254") ];
  const double           elapsed1=t1.elapsed();
  ensure("name found 1", !e1.first );

  // meauser second call, when result is read from cache
  const System::Timer    t2;
  const CachedDNS::Entry e2      =cache_[ ip("192.168.255.254") ];
  const double           elapsed2=t2.elapsed();
  ensure("name found 2", !e2.first );

  ensure("cached read took more time than query", elapsed1>elapsed2);
}

// test pruning only some of the elements
template<>
template<>
void testObj::test<6>(void)
{
  // create new entry
  cache_[ ip("74.125.39.103") ];
  cache_[ ip("74.125.39.104") ];
  cache_[ ip("74.125.39.105") ];
  // timeout
  delay(2);
  // add some new
  cache_[ ip("127.0.0.1") ];
  // remove entries - smoke test
  cache_.prune();
}

} // namespace tut
