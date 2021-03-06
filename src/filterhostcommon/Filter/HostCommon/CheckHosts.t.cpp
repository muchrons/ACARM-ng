/*
 * CheckHosts.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/HostCommon/CheckHosts.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Filter::HostCommon;
using namespace TestHelpers::Persistency;

namespace
{

struct TestStrategy: public Strategy
{
  TestStrategy(void):
    Strategy( TypeName("teststrategy"), InstanceName("teststrategyname"), 42)
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
  TestClass(void):
    ch_(&ts_)
  {
  }

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

  const Persistency::Host::IP &getHostIP(const Persistency::ConstGraphNodePtrNN &leaf) const
  {
    assert( leaf->isLeaf() );
    return (*leaf->getAlert()->getSourceHosts().begin())->getIP();
  }

  const TestStrategy ts_;
  CheckHosts         ch_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/HostCommon/CheckHosts");
} // unnamed namespace


namespace tut
{

// ensure reposnse is initially empty
template<>
template<>
void testObj::test<1>(void)
{
  ensure("initial response is not empty", ch_.out_.get()==NULL );
}

// check if getting any host sets default response
template<>
template<>
void testObj::test<2>(void)
{
  const GraphNodePtrNN leaf=makeLeaf(1, "1.2.3.4");
  ch_(leaf);
  ensure("no host selected", ch_.out_.get()!=NULL );
  ensure("invalid host selected", ch_.out_->getIP()==getHostIP(leaf) );
}

// test if operator is not blocking, when no host is initialy set
template<>
template<>
void testObj::test<3>(void)
{
  ch_( makeNewLeaf() );
  ensure("initial response is not empty", ch_.out_.get()==NULL );
  // now try setting it
  const GraphNodePtrNN leaf=makeLeaf(1, "1.2.3.4");
  ch_(leaf);
  ensure("no host selected", ch_.out_.get()!=NULL );
  ensure("invalid host selected", ch_.out_->getIP()==getHostIP(leaf) );
}

// test if it works fine, when secodn address does not match
template<>
template<>
void testObj::test<4>(void)
{
  ch_( makeLeaf(1, "1.2.3.4") );
  ensure("no host selected", ch_.out_.get()!=NULL );
  ch_( makeLeaf(2, "9.8.7.6") );
  ensure("host not de-sepected", ch_.out_.get()==NULL );
}

// test if it works fine when setting the same address for the second time
template<>
template<>
void testObj::test<5>(void)
{
  ch_( makeLeaf(1, "1.2.3.4") );
  ensure("no host selected", ch_.out_.get()!=NULL );
  ch_( makeLeaf(1, "1.2.3.4") );
  ensure("host de-sepected", ch_.out_.get()!=NULL );
}

// check if no match is possible after de-selecting one
template<>
template<>
void testObj::test<6>(void)
{
  ch_( makeLeaf(1, "1.2.3.4") );
  ensure("no host selected", ch_.out_.get()!=NULL );
  ch_( makeLeaf(2, "9.8.7.6") );
  ensure("host not de-sepected", ch_.out_.get()==NULL );
  // check after de-seelction
  ch_( makeLeaf(1, "1.2.3.4") );
  ensure("re-selection for 1.2.3.4", ch_.out_.get()==NULL );
  ch_( makeLeaf(2, "9.8.7.6") );
  ensure("re-selection for 9.8.7.6", ch_.out_.get()==NULL );
}

} // namespace tut
