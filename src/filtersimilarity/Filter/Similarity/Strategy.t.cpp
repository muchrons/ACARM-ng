/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/Similarity/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::Similarity;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    sampleLeaf_( mkLeaf("name 1") ),
    s_( "somename", Strategy::Params(123, 0.5) )
  {
  }

  GraphNodePtrNN mkLeaf(const char   *name="some name",
                        const char   *sip ="1.2.3.4",
                        const char   *tip ="2.3.4.5",
                        const char   *dns ="baszerr.org",
                        const double  certainty=0.42) const
  {
    return makeNewLeaf( makeNewAlert(name, sip, tip, dns, certainty) );
  }

  GraphNodePtrNN         sampleLeaf_;
  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Similarity/Strategy");
} // unnamed namespace

namespace tut
{


// test correlating of very simillar leafs
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN tmp=mkLeaf("other name");
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
}

// test if can correltate will return false for defferent leafs
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN tmp=mkLeaf("other", "4.5.6.7", "9.8.7.6", "wcss.pl", 0.98);
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test getting name of meta alert
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN tmp=mkLeaf("abC");
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  const string resp("[similarity] TODO");
  ensure_equals("invalid name", changed_[0]->getMetaAlert()->getName().get(), resp);
}

// test correlating 3rd element to meta alert
template<>
template<>
void testObj::test<4>(void)
{
  /*
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlert("some alert") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  const string resp("[similarity] some alert");
  ensure_equals("invalid name",
                changed_[0]->getMetaAlert()->getName().get(), resp);
  */
}

} // namespace tut
