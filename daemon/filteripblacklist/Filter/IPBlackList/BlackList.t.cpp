/*
 * BlackList.t.cpp
 *
 */
#include <tut.h>
#include <list>
#include <cassert>

#include "Filter/IPBlackList/BlackList.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  BlackList::IP ip(const char *str) const
  {
    assert(str!=NULL);
    return BlackList::IP::from_string(str);
  }

  std::list<BlackList::IP> ips_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/BlackList");
} // unnamed namespace


namespace tut
{

// test creating from empty list.
template<>
template<>
void testObj::test<1>(void)
{
  const BlackList bl( ips_.begin(), ips_.end() );
}

// test looking for exisitng entry
template<>
template<>
void testObj::test<2>(void)
{
  ips_.push_back( ip("3.2.3.4") );
  ips_.push_back( ip("2.2.3.4") );
  ips_.push_back( ip("1.2.3.4") );
  const BlackList bl( ips_.begin(), ips_.end() );
  ensure("ip1 not found", bl[ ip("1.2.3.4") ] );
  ensure("ip2 not found", bl[ ip("2.2.3.4") ] );
  ensure("ip3 not found", bl[ ip("3.2.3.4") ] );
}

// test looking for non-existing entry
template<>
template<>
void testObj::test<3>(void)
{
  ips_.push_back( ip("2.2.3.4") );
  ips_.push_back( ip("1.2.3.4") );
  const BlackList bl( ips_.begin(), ips_.end() );
  ensure("non-exisitng IP found", !bl[ ip("6.6.6.0") ] );
}

// test copyablity
template<>
template<>
void testObj::test<4>(void)
{
  ips_.push_back( ip("1.1.3.4") );
  ips_.push_back( ip("1.2.3.4") );
  BlackList bl1( ips_.begin(), ips_.end() );

  ips_.clear();
  ips_.push_back( ip("6.1.3.4") );
  ips_.push_back( ip("6.2.3.4") );
  const BlackList bl2( ips_.begin(), ips_.end() );

  bl1=bl2;
  ensure("expected IP 1 not found", bl1[ ip("6.1.3.4") ] );
  ensure("expected IP 2 not found", bl1[ ip("6.2.3.4") ] );
  ensure("unexpected IP 1 found",  !bl1[ ip("1.1.3.4") ] );
  ensure("unexpected IP 2 found",  !bl1[ ip("1.2.3.4") ] );
}

} // namespace tut
