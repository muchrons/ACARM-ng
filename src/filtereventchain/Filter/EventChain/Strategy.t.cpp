/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cmath>
#include <cassert>

#include "Filter/EventChain/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::EventChain;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    sampleLeaf_( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                    "4.3.2.1", NULL) ) ),
    s_( "somename", Strategy::Params(997, 0.42) )
  {
  }

  GraphNodePtrNN         sampleLeaf_;
  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/EventChain/Strategy");
} // unnamed namespace


namespace tut
{

// test if entry is not interesting when no target hosts are set
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN sourceOnlyLeaf(
        makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL, NULL, NULL) ) );
  s_.process(sourceOnlyLeaf, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test if entry is not interesting when no source hosts are set
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN targetOnlyLeaf(
      makeNewLeaf( makeNewAlertWithHosts(NULL, NULL, "1.2.3.4", NULL) ) );
  s_.process(targetOnlyLeaf, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test canCorrelate method
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("4.3.2.1", NULL,
                                                         "6.6.3.4", NULL ) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
}

// test if can correltate will return false for non-overlaping entries
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("6.6.3.4", NULL,
                                                         "6.6.3.4", NULL) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test getting name of meta alert
template<>
template<>
void testObj::test<5>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("4.3.2.1", NULL,
                                                         "6.6.3.4", NULL ) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);

  const string resp("[eventchain] chain of events");
  ensure_equals("invalid name", changed_[0]->getMetaAlert()->getName().get(), resp);
}

// test creating config paramters
template<>
template<>
void testObj::test<6>(void)
{
  const Strategy::Params p(123, 0.11);
  ensure_equals("invalid timeout", p.timeout_,  123);
  ensure_equals("invalid delta",   p.priDelta_, 0.11);
}

// test negative delta
template<>
template<>
void testObj::test<7>(void)
{
  const Strategy::Params p(123, -0.11);
  ensure_equals("invalid timeout", p.timeout_,   123);
  ensure_equals("invalid delta",   p.priDelta_, -0.11);
}

// test correlation when multiple hosts are present
template<>
template<>
void testObj::test<8>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("6.6.3.4", "4.3.2.1",
                                                         "6.6.3.4", "9.8.7.6") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("correlation failed for multiple hosts", changed_.size(), 1u);
}

// test if correlation of chain consisting of 3 alerts - 3rd added in front
template<>
template<>
void testObj::test<9>(void)
{
  {
    GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("6.6.3.4", NULL,
                                                           "6.6.3.5", NULL) ) );
    s_.process(tmp, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
  }

  {
    GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("6.6.3.5", NULL,
                                                           "1.2.3.4", NULL ) ) );
    s_.process(tmp, changed_);
    ensure_equals("nothing has changed", changed_.size(), 1u);
  }
  GraphNodePtrNN node=changed_[0];
  changed_.clear();

  s_.process(sampleLeaf_, changed_);
  ensure_equals("correlation of chain of length 3 failed", changed_.size(), 1u);
  ensure("old node not used for correlation", node.get()==changed_[0].get() );
}

// test if correlation of chain consisting of 3 alerts - 3rd added on back
template<>
template<>
void testObj::test<10>(void)
{
  {
    GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("4.3.2.1", NULL,
                                                           "6.6.3.5", NULL) ) );
    s_.process(tmp, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
  }

  {
    GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("6.6.3.5", NULL,
                                                           "6.6.3.6", NULL ) ) );
    s_.process(tmp, changed_);
    ensure_equals("nothing has changed", changed_.size(), 1u);
  }
  GraphNodePtrNN node=changed_[0];
  changed_.clear();

  s_.process(sampleLeaf_, changed_);
  ensure_equals("correlation of chain of length 3 failed", changed_.size(), 1u);
  ensure("old node not used for correlation", node.get()==changed_[0].get() );
}

// test if correlation changes priotity delta
template<>
template<>
void testObj::test<11>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("4.3.2.1", NULL,
                                                         "6.6.3.4", NULL ) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);

  ensure("invalid priority delta", fabs(changed_[0]->getMetaAlert()->getSeverityDelta() - 0.42)<0.01 );
}

// test if correlation does not occure for entries that have the same source and IP addresses
template<>
template<>
void testObj::test<12>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("4.3.2.1", NULL,
                                                         "4.3.2.1", NULL ) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("correlated with alert with same src/dst address", changed_.size(), 0u);
}

} // namespace tut
