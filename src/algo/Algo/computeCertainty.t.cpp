/*
 * computeCertainty.t.cpp
 *
 */
#include <tut.h>

#include "Algo/computeCertainty.hpp"
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
    alertCertainty_(0.42)
  {
  }

  // check if given doubles are the same, with some precission.
  void checkMatch(const double v, const double e) const
  {
    const double eps=0.00001;
    // if-trick allows checking with range and if does not match, show
    // computed/expected results
    if( fabs(v-e)>eps )
      tut::ensure_equals("invalid value", v, e);
  }

  const double alertCertainty_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/computeCertainty");
} // unnamed namespace


namespace tut
{

// compute certainty of a leaf
template<>
template<>
void testObj::test<1>(void)
{
  checkMatch( computeCertainty( makeNewLeaf() ), alertCertainty_);
}

// compute certainty of a node with 2 leafs
template<>
template<>
void testObj::test<2>(void)
{
  checkMatch( computeCertainty( makeNewNode() ), 2*alertCertainty_/2.0 + 1*0.2);
}

// compute certainty on non-unique tree
template<>
template<>
void testObj::test<3>(void)
{
  checkMatch( computeCertainty( makeNewTree2() ), 3*alertCertainty_/3.0 + 4*0.2);
}

// compute certainty via raw pointer
template<>
template<>
void testObj::test<4>(void)
{
  checkMatch( computeCertainty( makeNewTree2().get() ), 3*alertCertainty_/3.0 + 4*0.2);
}

// test for exception when got NULL
template<>
template<>
void testObj::test<5>(void)
{
  const GraphNode *null=NULL;
  try
  {
    computeCertainty(null);
    fail("no exception on NULL");
  }
  catch(const Algo::Exception &ex)
  {
    // this is expected.
  }
}

} // namespace tut
