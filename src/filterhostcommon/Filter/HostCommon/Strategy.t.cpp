/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/HostCommon/Strategy.hpp"
#include "Filter/HostCommon/Exception.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::HostCommon;
using namespace TestHelpers::Persistency;

namespace
{

struct TestStrategy: public Strategy
{
  TestStrategy(void):
    Strategy("teststrategy", "teststrategyname", 42)
  {
  }

  virtual const Persistency::Alert::Hosts &getHostsArray(ConstNode node) const
  {
    return node->getAlert()->getSourceHosts();
  }

  virtual MetaAlert::Name getMetaAlertName(ConstHostPtrNN /*h*/) const
  {
    return Persistency::MetaAlert::Name("hakuna matata");
  }
}; // struct TestStrategy



struct TestClass: private TestHelpers::Persistency::TestStubs
{
  AlertPtrNN makeAlertWithHost(const char *host, const char *os) const
  {
    const Alert::Analyzers sa( makeNewAnalyzer() );
    const Host::Netmask_v4 mask(mask4_bytes);
    Alert::Hosts           hosts;
    hosts.push_back( makeNewHost4(host, &mask, os) );
    return AlertPtrNN( new Alert("alert 123",
                                 sa,
                                 NULL,
                                 Timestamp(),
                                 Severity(SeverityLevel::INFO),
                                 Certainty(0.42),
                                 "some test alert",
                                 hosts,
                                 Alert::Hosts() ) );
  }

  TestStrategy::Node makeLeaf(const unsigned int id, const char *host, const char *os="Linux") const
  {
    assert(host!=NULL);
    IO::ConnectionPtrNN conn( IO::create() );
    IO::Transaction     t( conn->createNewTransaction("make_leaf_trans") );
    return GraphNodePtrNN( new GraphNode( makeAlertWithHost(host, os), id, conn, t) );
  }

  TestStrategy::Node makeNode(void) const
  {
    return makeNewNode( makeLeaf(1, "1.2.3.4"), makeLeaf(2, "1.2.3.4") );
  }

  TestStrategy::ChangedNodes changed_;
  TestStrategy               ts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/HostCommon/Strategy");
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
    const TestStrategy::Node tmp=makeLeaf(i+10, tab[i]);
    ts_.process(tmp, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
  }
}

// test running when self-entry has been found
template<>
template<>
void testObj::test<2>(void)
{
  TestStrategy::Node n=makeLeaf(1, "6.6.6.6");
  for(int i=0; i<2; ++i)
  {
    ts_.process(n, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
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
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
  }
}

// try correlating two hosts, with different os'es
template<>
template<>
void testObj::test<4>(void)
{
  TestStrategy::Node l1=makeLeaf(1, "1.2.3.4", "os1");
  ts_.process(l1, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  TestStrategy::Node l2=makeLeaf(1, "1.2.3.4", "os2");
  ts_.process(l2, changed_);
  ensure_equals("no correlation has been done", changed_.size(), 1u);
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
  virtual MetaAlert::Name getMetaAlertName(ConstHostPtrNN /*h*/) const
  {
    throw Filter::HostCommon::Exception(SYSTEM_SAVE_LOCATION, "just", "testing");
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
  ts.process( makeLeaf(1, "1.2.3.4", "os1"), changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);
  ts.process( makeLeaf(2, "1.2.3.4", "os2"), changed_);    // nothing should happen here
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test adding elements to already exisitng node.
template<>
template<>
void testObj::test<6>(void)
{
  TestStrategy::Node n=makeNode();
  ts_.process(n, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  TestStrategy::Node l=makeLeaf(1, "1.2.3.4");
  ts_.process(l, changed_);
  ensure_equals("nodes not changed", changed_.size(), 1u);
  // check pointers - according to rules, new entry should be created in this context
  ensure("invalid node has been changed", changed_.begin()->get()!=n.get() );
}

} // namespace tut
