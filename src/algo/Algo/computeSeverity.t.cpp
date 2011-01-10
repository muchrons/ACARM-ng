/*
 * computeSeverity.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/create.hpp"
#include "Algo/computeSeverity.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    alertSeverity_( SeverityLevel::INFO )
  {
  }

  // check if given doubles are the same, with some precission.
  void checkMatch(const double v, const double e, const double eps) const
  {
    // if-trick allows checking with range and if does not match, show
    // computed/expected results
    if( fabs(v-e)>eps )
      tut::ensure_equals("invalid value", v, e);
  }

  const int alertSeverity_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/computeSeverity");
} // unnamed namespace


namespace tut
{

// compute severity of a leaf
template<>
template<>
void testObj::test<1>(void)
{
  checkMatch( computeSeverity( makeNewLeaf() ), 1.04, 0.05);
}

// compute severity of a node with 2 leafs
template<>
template<>
void testObj::test<2>(void)
{
  checkMatch( computeSeverity( makeNewNode() ), 1.18, 0.05);
}

// compute severity on non-unique tree
template<>
template<>
void testObj::test<3>(void)
{
  checkMatch( computeSeverity( makeNewTree2() ), 1.51, 0.05);
}

// test for bug (failing assertion)
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN                leaf=makeNewLeaf();
  std::auto_ptr<IO::Connection> conn=IO::create();
  ensure("NULL-connection", conn.get()!=NULL );
  IO::TransactionAPIAutoPtr     tApi=conn->createNewTransaction("leaf_update_severity");
  IO::Transaction               t(tApi);
  conn->metaAlert(leaf->getMetaAlert(), t)->updateSeverityDelta(0.1);
  t.commit();
  // following code must not crash
  checkMatch( computeSeverity(leaf), 1.14, 0.05);
}

} // namespace tut
