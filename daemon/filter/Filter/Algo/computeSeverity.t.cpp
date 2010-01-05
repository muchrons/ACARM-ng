/*
 * computeSeverity.t.cpp
 *
 */
#include <tut.h>

#include "Filter/Algo/computeSeverity.hpp"
#include "Filter/TestHelpers.t.hpp"

using namespace Filter::Algo;
using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    alertSeverity_( SeverityLevel::INFO )
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

  int alertSeverity_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Algo/computeSeverity");
} // unnamed namespace


namespace tut
{

// compute severity of a leaf
template<>
template<>
void testObj::test<1>(void)
{
  checkMatch( computeSeverity( th_makeLeaf() ), alertSeverity_);
}

// compute severity of a node with 2 leafs
template<>
template<>
void testObj::test<2>(void)
{
  checkMatch( computeSeverity( th_makeNode() ),
              2*alertSeverity_/2.0 + 1*0.1);
}

// compute severity on non-unique tree
template<>
template<>
void testObj::test<3>(void)
{
  checkMatch( computeSeverity( th_makeTree2() ),
              3*alertSeverity_/3.0 + 4*0.1);
}

} // namespace tut
