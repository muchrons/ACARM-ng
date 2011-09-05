/*
 * OnAnalyzer.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnAnalyzer.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnAnalyzer>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnAnalyzer");
} // unnamed namespace


namespace tut
{

// test processing last element
template<>
template<>
void testObj::test<1>(void)
{
  TestParams p(Path("analyzer.name"), cb_);
  ++p;
  ensureThrow(*analyzer_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("analyzer.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*analyzer_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("analyzer.enemyunknown"), cb_);
  ensureThrow(*analyzer_, p);
}

// test getting id
template<>
template<>
void testObj::test<4>(void)
{
  ensureProc("invalid ID", *analyzer_, "analyzer.id", "666");
}

// test getting name
template<>
template<>
void testObj::test<5>(void)
{
  ensureProc("invalid name", *analyzer_, "analyzer.name", "analyzer1");
}

// test getting version
template<>
template<>
void testObj::test<6>(void)
{
  ensureProc("invalid version", *analyzer_, "analyzer.version", "4.2");
}

// test getting version when NULL
template<>
template<>
void testObj::test<7>(void)
{
  ensureProcNull("no NULL on NULL version", *analyzerNull_, "analyzer.version", "version");
}

// test getting OS
template<>
template<>
void testObj::test<8>(void)
{
  ensureProc("invalid OS", *analyzer_, "analyzer.operatingsystem", "Linux/Debian");
}

// test getting OS when NULL
template<>
template<>
void testObj::test<9>(void)
{
  ensureProcNull("no NULL on NULL OS", *analyzerNull_, "analyzer.operatingsystem", "operatingsystem");
}

// test getting IP
template<>
template<>
void testObj::test<10>(void)
{
  ensureProc("invalid IP", *analyzer_, "analyzer.ip", "1.2.3.4");
}

// test getting IP when NULL
template<>
template<>
void testObj::test<11>(void)
{
  ensureProcNull("no NULL on NULL IP", *analyzerNull_, "analyzer.ip", "ip");
}

// test exception on invalid root name
template<>
template<>
void testObj::test<12>(void)
{
  TestParams p(Path("invalidanalyzername.name"), cb_);
  ensureThrow(*analyzer_, p);
}

} // namespace tut
