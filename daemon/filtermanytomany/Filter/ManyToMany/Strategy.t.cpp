/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/ManyToMany/Strategy.hpp"
#include "Filter/ManyToMany/Exception.hpp"
#include "Filter/ManyToMany/CommonTestBase.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::ManyToMany;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public CommonTestBase
{
  TestClass(void):
    sampleLeaf_( makeLeaf( makeAlert("1.2.3.4", "2.3.4.5",
                                     "1.2.3.4", "9.8.7.6") ) )
  {
  }

  GraphNodePtrNN         sampleLeaf_;
  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/ManyToMany/Strategy");
} // unnamed namespace


namespace tut
{

// test if entry is not interesting when no target hosts are set
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN sourceOnlyLeaf( makeLeaf( makeAlert("1.2.3.4", NULL,
                                                     NULL,      NULL) ) );
  s_.process(sourceOnlyLeaf, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0);
}

// test if entry is not interesting when no source hosts are set
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN targetOnlyLeaf( makeLeaf( makeAlert(NULL,      NULL,
                                                     "1.2.3.4", NULL) ) );
  s_.process(targetOnlyLeaf, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0);
}

// test canCorrelate method
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN tmp( makeLeaf( makeAlert("6.6.3.4", "2.3.4.5",
                                          "6.6.3.4", "9.8.7.6") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 1);
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

} // namespace tut
