/*
 * Strategy.t.cpp
 *
 */
// TODO
#if 0
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/DNSResolver/Strategy.hpp"
#include "Filter/DNSResolver/Exception.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::DNSResolver;
using namespace TestHelpers::Persistency;

namespace
{

struct TestStrategy: public Strategy
{
  TestStrategy(void):
    Strategy("teststrategy", 42)
  {
  }

  virtual const Persistency::Alert::ReportedHosts &getReportedHostsArray(
                                                          const Node node) const
  {
    return node->getAlert().getReportedSourceHosts();
  }

  virtual MetaAlert::Name getMetaAlertName(const HostPtrNN /*h*/) const
  {
    return Persistency::MetaAlert::Name("hakuna matata");
  }
}; // struct TestStrategy



struct TestClass: private TestHelpers::Persistency::TestStubs
{
  AlertPtrNN makeAlertWithHost(const char *host, const char *os) const
  {
    const Alert::SourceAnalyzers sa( makeNewAnalyzer() );
    const Host::Netmask_v4       mask(mask4_bytes);
    Alert::ReportedHosts         hosts;
    hosts.push_back( makeNewHost4(host, &mask, os) );
    return AlertPtrNN( new Alert("alert 123",
                                 sa,
                                 NULL,
                                 Timestamp(),
                                 Severity(SeverityLevel::INFO),
                                 Certainty(0.42),
                                 "some test allert",
                                 hosts,
                                 Alert::ReportedHosts() ) );
  }

  TestStrategy::Node makeLeaf(const char *host, const char *os="Linux") const
  {
    assert(host!=NULL);
    IO::ConnectionPtrNN conn( IO::create() );
    IO::Transaction     t( conn->createNewTransaction("make_leaf_trans") );
    return GraphNodePtrNN( new GraphNode( makeAlertWithHost(host, os), conn, t) );
  }

  TestStrategy::Node makeNode(void) const
  {
    return makeNewNode( makeLeaf("1.2.3.4"), makeLeaf("1.2.3.4") );
  }

  TestStrategy::ChangedNodes changed_;
  TestStrategy               ts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/Strategy");
} // unnamed namespace


namespace tut
{

// test runs when hosts differ
template<>
template<>
void testObj::test<1>(void)
{
  const char *tab[]={"1.2.3.4", "2.3.4.5"};
  for(int i=0; i<2; ++i)
  {
    const TestStrategy::Node tmp=makeLeaf(tab[i]);
    ts_.process(tmp, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0);
  }
}

// test running when self-entry has been found
template<>
template<>
void testObj::test<2>(void)
{
  TestStrategy::Node n=makeLeaf("6.6.6.6");
  for(int i=0; i<2; ++i)
  {
    ts_.process(n, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0);
  }
}

// test when no common host in node is found.
template<>
template<>
void testObj::test<3>(void)
{
  for(int i=0; i<2; ++i)
  {
    ts_.process( makeNewLeaf(), changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0);
  }
}

// try correlating two hosts, with different os'es
template<>
template<>
void testObj::test<4>(void)
{
  TestStrategy::Node l1=makeLeaf("1.2.3.4", "os2");
  ts_.process(l1, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0);

  TestStrategy::Node l2=makeLeaf("1.2.3.4", "os2");
  ts_.process(l2, changed_);
  ensure_equals("no correlation has been done", changed_.size(), 1);
  // comapre pointers - new node should have been created here
  ensure("invalid element in changed list",
             changed_.begin()->get()!=l1.get() &&
             changed_.begin()->get()!=l2.get()    );
  ensure_equals("invalid name",
                (*changed_.begin())->getMetaAlert()->getName().get(),
                string("hakuna matata") );
}

namespace
{
struct ThrowStrategy: public TestStrategy
{
  virtual MetaAlert::Name getMetaAlertName(const HostPtrNN /*h*/) const
  {
    throw Filter::DNSResolver::Exception(SYSTEM_SAVE_LOCATION, "just", "testing");
    return "alicehasacat";  // we never reach here
  }
}; // struct ThrowStrategy
} // unnamed namespace

// test if exceptions inside the loop does not get outside!
template<>
template<>
void testObj::test<5>(void)
{
  ThrowStrategy ts;
  ts.process( makeLeaf("1.2.3.4", "os1"), changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0);
  ts.process( makeLeaf("1.2.3.4", "os2"), changed_);    // nothing should happen here
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0);
}

// test adding elements to already exisitng node.
template<>
template<>
void testObj::test<6>(void)
{
  TestStrategy::Node n=makeNode();
  ts_.process(n, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0);

  TestStrategy::Node l=makeLeaf("1.2.3.4");
  ts_.process(l, changed_);
  ensure_equals("nodes not changed", changed_.size(), 1);
  // check pointers
  ensure("invalid node has been changed", changed_.begin()->get()==n.get() );
}

} // namespace tut
#endif
