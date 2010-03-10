/*
 * CachedEntry.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Filter/DNSResolver/CachedEntry.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Filter::DNSResolver;

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

factory tf("Filter/DNSResolver/CachedEntry");
} // unnamed namespace


namespace tut
{

// test timeout
template<>
template<>
void testObj::test<1>(void)
{
  const CachedEntry ce( ip("127.0.0.1"), 21);
  ensure("invalid timeout", time(NULL)+21-ce.time_<20 );
}

// test reverse-DNS
template<>
template<>
void testObj::test<2>(void)
{
  const CachedEntry ce( ip("127.0.0.1"), 21);
  ensure("reverse DNS for localhost failed", ce.hasName() );
  ensure("invalid name", strstr( ce.name_.get(), "localhost" )!=NULL );
}

// reverse-dns for unexisting entry
template<>
template<>
void testObj::test<3>(void)
{
  const CachedEntry ce( ip("192.168.255.254"), 21); // this IP most likely will not have reverse DNS
  ensure("reverse DNS didn't failed for non-existings IP", !ce.hasName() );
}

} // namespace tut
