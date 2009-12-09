/*
 * Analyzer.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <memory>

#include "Persistency/Analyzer.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass
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
  ensure("invalid host", a->getHost()==host.get() );
}

// test creating with NULL host
template<>
template<>
void testObj::test<2>(void)
{
  HostPtr host;
  const AnalyzerPtr a=makeNewAnalyzer("analyzer1", host);
  ensure_equals("invalid name", a->getName().get(), string("analyzer1") );
  ensure("invalid host", a->getHost()==NULL);
}

} // namespace tut
