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

  template<typename T>
  void testThrow(const T &e, TestParams &p) const
  {
    try
    {
      OnTerm::process(e, p);                // should throw
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
  testThrow(string("aaa"), pLast_);
}

// test exception when not-last element is set
template<>
template<>
void testObj::test<2>(void)
{
  testThrow(string("aaa"), p_);
}

// test collection
template<>
template<>
void testObj::test<3>(void)
{
  vector<int> v(10, 42);
  testThrow(v, pLast_);
}

// test some term value
template<>
template<>
void testObj::test<4>(void)
{
  OnTerm::process(string("yellow"), pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "yellow");
}

// test processing MD5Sum
template<>
template<>
void testObj::test<5>(void)
{
  const std::string         md5Str("d0295a7f36fa2d19d4ce40382c8d139f");
  const Persistency::MD5Sum md5( Persistency::MD5Sum::createFromString(md5Str.c_str()) );
  OnTerm::process(md5, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, md5Str);
}

} // namespace tut
