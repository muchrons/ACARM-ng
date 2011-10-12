/*
 * OnAlert.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnAlert.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnAlert>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnAlert");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensureProc("invalid name", *alert_, "alert.name", "alertname");
}

// test analyzer's count
template<>
template<>
void testObj::test<2>(void)
{
  ensureProcSize("invalid analyzers count", *alert_, "alert.analyzers.size", 1);
}

// test processing last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("alert.name"), cb_);
  ++p;
  ensureThrow(*alert_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("alert.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*alert_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("alert.enemyunknown"), cb_);
  ensureThrow(*alert_, p);
}

// test accessing via colleciton index
template<>
template<>
void testObj::test<6>(void)
{
  ensureProc("error on colleciton index", *alert_, "42.name", "alertname");
}

// test getting detection time
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid detection time", *alert_, "alert.detected", "112233");
}

// test getting detection time when null
template<>
template<>
void testObj::test<8>(void)
{
  ensureProcNull("invalid null handle on detection time", *alertNull_, "alert.detected", "detected");
}

// test getting creation time
template<>
template<>
void testObj::test<9>(void)
{
  ensureProc("invalid creation time", *alert_, "alert.created", "556677");
}

// test getting severity
template<>
template<>
void testObj::test<10>(void)
{
  ensureProc("invalid severity", *alert_, "alert.severity", "info");
}

// test getting certainty
template<>
template<>
void testObj::test<11>(void)
{
  ensureProc("invalid certaintly", *alert_, "alert.certainty", "0.5");
}

// test getting description
template<>
template<>
void testObj::test<12>(void)
{
  ensureProc("invalid description", *alert_, "alert.description", "some text info");
}

// test getting source hosts count
template<>
template<>
void testObj::test<13>(void)
{
  ensureProcSize("invalid source hosts count", *alert_, "alert.sources.size", 2);
}

// test getting destination hosts size
template<>
template<>
void testObj::test<14>(void)
{
  ensureProcSize("invalid target hosts count", *alert_, "alert.targets.size", 1);
}

// test getting random analyzer's random element
template<>
template<>
void testObj::test<15>(void)
{
  ensureProc("invalid analyzer's id", *alert_, "alert.analyzers.0.id", "666");
}

// test getting random hosts's random element
template<>
template<>
void testObj::test<16>(void)
{
  ensureProc("invalid hosts's OS", *alert_, "alert.sources.0.os", "Linux");
}

// test exception on unknown root name
template<>
template<>
void testObj::test<17>(void)
{
  TestParams p(Path("invalidalertname.name"), cb_);
  ensureThrow(*alert_, p);
}

} // namespace tut
