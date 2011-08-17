/*
 * OnReferenceURL.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnReferenceURL.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnReferenceURL>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnReferenceURL");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensureProc("invalid name", *url_, "referenceurl.name", "evil site");
}

// test getting URL
template<>
template<>
void testObj::test<2>(void)
{
  ensureProc("invalid url", *url_, "referenceurl.url", "http://evil.one");
}

// test processing for last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("referenceurl.url"), cb_);
  ++p;
  ensureThrow(*url_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("referenceurl.url"), cb_);
  ++p;
  ++p;
  ensureThrow(*url_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("referenceurl.enemyunknown"), cb_);
  ensureThrow(*url_, p);
}

// test accessing via collection index
template<>
template<>
void testObj::test<6>(void)
{
  ensureProc("failed for colleciton index", *url_, "42.url", "http://evil.one");
}

// test error when path is too long
template<>
template<>
void testObj::test<7>(void)
{
  TestParams p(Path("referenceurl.url.TOOLONG"), cb_);
  ensureThrow(*url_, p);
}

// test exception on invalid root name
template<>
template<>
void testObj::test<8>(void)
{
  TestParams p(Path("invalidreferenceurlname.url"), cb_);
  ensureThrow(*url_, p);
}

} // namespace tut
