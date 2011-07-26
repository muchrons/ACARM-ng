/*
 * OnReferenceURL.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnReferenceURL.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    r_("somename", "http://evil.one")
  {
  }

  void ensureThrow(TestParams &p)
  {
    try
    {
      OnReferenceURL::process(r_, p);
      tut::fail("call didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  TestParams::ResultCallback cb_;
  ReferenceURL               r_;
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
  TestParams p(Path("referenceurl.name"), cb_);
  OnReferenceURL::process(r_, p);
  ensure_equals("invalid name", cb_.lastValue_, "somename");
}

// test getting URL
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("referenceurl.url"), cb_);
  OnReferenceURL::process(r_, p);
  ensure_equals("invalid url", cb_.lastValue_, "http://evil.one");
}

// test processing for last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("referenceurl.url"), cb_);
  ++p;
  ensureThrow(p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("referenceurl.url"), cb_);
  ++p;
  ++p;
  ensureThrow(p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("referenceurl.enemyunknown"), cb_);
  ensureThrow(p);
}

// test error when invalid root is requested
template<>
template<>
void testObj::test<6>(void)
{
  TestParams p(Path("whatisthat.url"), cb_);
  ensureThrow(p);
}

// test error when path is too long
template<>
template<>
void testObj::test<7>(void)
{
  TestParams p(Path("referenceurl.url.TOOLONG"), cb_);
  ensureThrow(p);
}

} // namespace tut
