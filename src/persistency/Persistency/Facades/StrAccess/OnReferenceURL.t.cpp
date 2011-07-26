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
  TestParams p(Path("x.name"), cb_);
  OnReferenceURL::process(r_, p);
  ensure_equals("invalid result", cb_.lastValue_, "somename");
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
