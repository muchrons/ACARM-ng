/*
 * Analyzer.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <memory>

#include "Persistency/Analyzer.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Analyzer");
} // unnamed namespace


namespace tut
{

// test creating regular analyzer
template<>
template<>
void testObj::test<1>(void)
{
  HostPtr host=makeNewHost();
  const AnalyzerPtr a=makeNewAnalyzer("analyzer1", host);
  ensure_equals("invalid name", a->getName().get(), string("analyzer1") );
  ensure("invalid host", a->getHost().get()==host.get() );
}

// test creating with NULL host
template<>
template<>
void testObj::test<2>(void)
{
  HostPtr host;
  const AnalyzerPtr a=makeNewAnalyzer("analyzer1", host);
  ensure_equals("invalid name", a->getName().get(), string("analyzer1") );
  ensure("invalid host", a->getHost().get()==NULL);
}

// test comparing for different name
template<>
template<>
void testObj::test<3>(void)
{
  const AnalyzerPtrNN a1a=makeNewAnalyzer("analyzer1", makeNewHost() );
  const AnalyzerPtrNN a1b=makeNewAnalyzer("analyzer1", makeNewHost() );
  const AnalyzerPtrNN a2 =makeNewAnalyzer("analyzer2", makeNewHost() );
  TestHelpers::checkEquality(*a1a, *a1b, *a2);
}

// test comparing for different host
template<>
template<>
void testObj::test<4>(void)
{
  const AnalyzerPtrNN a1=makeNewAnalyzer("analyzer1", makeNewHost() );
  const AnalyzerPtrNN a2=makeNewAnalyzer("analyzer2", HostPtr() );
  TestHelpers::checkEquality(*a1, *a2);
}

} // namespace tut
