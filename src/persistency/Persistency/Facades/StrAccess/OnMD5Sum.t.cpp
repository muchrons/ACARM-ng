/*
 * OnMD5Sum.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnMD5Sum.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    v_("78b9861f74e15d7d0f077ba22421b8e4"),
    md5_( MD5Sum::createFromString(v_.c_str()) )
  {
  }

  void ensureThrow(TestParams &p)
  {
    try
    {
      OnMD5Sum::process(md5_, p);
      tut::fail("call didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  TestParams::ResultCallback cb_;
  const std::string          v_;
  MD5Sum                     md5_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnMD5Sum");
} // unnamed namespace


namespace tut
{

// test getting value
template<>
template<>
void testObj::test<1>(void)
{
  TestParams p(Path("x.md5sum"), cb_);
  ++p;
  OnMD5Sum::process(md5_, p);
  ensure_equals("invalid name", cb_.lastValue_, v_);
}

// test processing non-last element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("md5sum.x"), cb_);
  ensureThrow(p);
}

// test processing for end element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("x.md5sum"), cb_);
  ++p;
  ++p;
  ensureThrow(p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("x.oops"), cb_);
  ensureThrow(p);
}

} // namespace tut
