/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/OneToOne/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::OneToOne;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    sampleLeaf_( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                    "2.3.4.5", NULL) ) ),
    s_(997)
  {
  }

  GraphNodePtrNN         sampleLeaf_;
  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/OneToOne/Strategy");
} // unnamed namespace


namespace tut
{

// test if entry is not interesting when no target host is set
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

// test if entry is not interesting when no source host is set
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
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                         "2.3.4.5", NULL) ) );
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
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                         "9.3.4.5", NULL) ) );
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
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                         "2.3.4.5", NULL) ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);

  const string resp("[one2one] multiple attacks from 1.2.3.4 host "
                    "on 2.3.4.5 host");
  ensure_equals("invalid name",
                changed_[0]->getMetaAlert()->getName().get(), resp);
}

} // namespace tut
