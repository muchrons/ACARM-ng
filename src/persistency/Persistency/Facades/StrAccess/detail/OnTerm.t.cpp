/*
 * OnTerm.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "Persistency/Facades/StrAccess/detail/OnTerm.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;

namespace
{
struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_),
    pLast_(Path("a.b"), cb_)
  {
    ++pLast_;
    assert(pLast_.isEnd()==false);
    assert(pLast_.hasNext()==false);
  }

  void testThrow(TestParams &p) const
  {
    try
    {
      OnTerm::process(string("aaa"), p);        // should throw
      tut::fail("process() didn't failed");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  TestParams::ResultCallback cb_;
  TestParams                 p_;
  TestParams                 pLast_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/detail/OnTerm");
} // unnamed namespace


namespace tut
{

// test exception when end is reached
template<>
template<>
void testObj::test<1>(void)
{
  ++pLast_;
  assert(pLast_.isEnd());
  testThrow(pLast_);
}

// test exception when not-last element is set
template<>
template<>
void testObj::test<2>(void)
{
  testThrow(p_);
}

// test collection
template<>
template<>
void testObj::test<3>(void)
{
  vector<int> v(10, 42);
  assert(v.size()==10);
  OnTerm::process(v, pLast_);
  ensure_equals("invalid value returned", cb_.lastSize_, 10);
}

} // namespace tut
