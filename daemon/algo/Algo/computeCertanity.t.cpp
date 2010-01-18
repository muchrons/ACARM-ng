/*
 * computeCertanity.t.cpp
 *
 */
#include <tut.h>

#include "Algo/computeCertanity.hpp"
#include "Algo/TestHelpers.t.hpp"

using namespace Algo;
using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    alertCertanity_(0.42)
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

  const double alertCertanity_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/computeCertanity");
} // unnamed namespace


namespace tut
{

// compute certanity of a leaf
template<>
template<>
void testObj::test<1>(void)
{
  checkMatch( computeCertanity( th_makeLeaf() ), alertCertanity_);
}

// compute certanity of a node with 2 leafs
template<>
template<>
void testObj::test<2>(void)
{
  checkMatch( computeCertanity( th_makeNode() ),
              2*alertCertanity_/2.0 + 1*0.2);
}

// compute certanity on non-unique tree
template<>
template<>
void testObj::test<3>(void)
{
  checkMatch( computeCertanity( th_makeTree2() ),
              3*alertCertanity_/3.0 + 4*0.2);
}

} // namespace tut
