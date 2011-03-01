/*
 * analyzer.t.cpp
 *
 */
#include <tut.h>

#include "System/Math/compareFP.hpp"
#include "Algo/Diff/analyzer.hpp"

#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo::Diff;
using namespace ::Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/analyzer");
} // unnamed namespace


namespace tut
{

// test comparing identical analyzers
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s=compare( *makeNewAnalyzer(), *makeNewAnalyzer() );
  ensure("value too small", System::Math::compareFP<double>(s.get(), 1, 3) );
}

// test comapring non-identical analyzers
template<>
template<>
void testObj::test<2>(void)
{
  const Similarity s=compare( *makeNewAnalyzer("abc"), *makeNewAnalyzer("def") );
  ensure("invalid value", System::Math::compareFP<double>(s.get(), 4/5.0, 3) );
}

} // namespace tut
