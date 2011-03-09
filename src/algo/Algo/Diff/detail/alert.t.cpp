/*
 * alert.t.cpp
 *
 */
#include <tut.h>

#include "System/Math/compareFP.hpp"
#include "Algo/Diff/detail/alert.hpp"

#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;
using namespace ::Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/alert");
} // unnamed namespace


namespace tut
{

// check if identical severities are the same
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( *makeNewAlert(), *makeNewAlert() );
  ensure("value too small", System::Math::compareFP<double>(s.get(), 1, 3) );
}

// test slightly different alerts
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( *makeNewAlert("abc", "1.2.3.4"), *makeNewAlert("qqq", "6.6.6.0") );
  ensure("value too small", s.get()>0.5 );
  ensure("value too large", s.get()<0.9 );
}

} // namespace tut
