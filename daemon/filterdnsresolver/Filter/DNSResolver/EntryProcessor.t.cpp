/*
 * EntryProcessor.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/DNSResolver/EntryProcessor.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter;
using namespace Filter::DNSResolver;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
    cache_(42),
    bf_(conn_, changed_, "testdnsresolver"),
    ep_(&cache_, &bf_)
  {
    assert( changed_.size()==0 );
  }

  Persistency::IO::ConnectionPtrNN conn_;
  CachedDNS                        cache_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  EntryProcessor                   ep_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/EntryProcessor");
} // unnamed namespace


namespace tut
{

// test empty run (no hosts)
template<>
template<>
void testObj::test<1>(void)
{
  ep_( makeNewLeaf() );
  ensure_equals("something has been changed", changed_.size(), 0);
}

// test run when some source hosts have reverse dns'
template<>
template<>
void testObj::test<2>(void)
{
  ep_( makeNewLeaf("127.0.0.1", NULL, false) );
  ensure_equals("nothing has been changed", changed_.size(), 1);
  const char *tmp=changed_.at(0)->getAlert()->getReportedSourceHosts().at(0)->getName().get();
  ensure("invalid dns found", strstr(tmp, "localhost")!=NULL );
}

// test run when there are hosts but no reverse dns mappings
template<>
template<>
void testObj::test<3>(void)
{
  ep_( makeNewLeaf("192.168.255.254", "192.168.255.253", false) );
  ensure_equals("something has been changed", changed_.size(), 0);
}

// test run when some target hosts have reverse dns'
template<>
template<>
void testObj::test<4>(void)
{
  ep_( makeNewLeaf(NULL, "127.0.0.1", false) );
  ensure_equals("nothing has been changed", changed_.size(), 1);
  const char *tmp=changed_.at(0)->getAlert()->getReportedTargetHosts().at(0)->getName().get();
  ensure("invalid dns found", strstr(tmp, "localhost")!=NULL );
}

} // namespace tut
