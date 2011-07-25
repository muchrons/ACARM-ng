/*
 * Term.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "Persistency/Facades/StrAccess/detail/Term.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;
#if 0                       /// TODO                

namespace
{
struct TestClass
{
  TestClass(void):
    p_( Path("a.b"), cb_),
    pLast_( Path("a.b"), cb_)
  {
    ++pLast_;
    assert(pLast_.isEnd()==false);
    assert(pLast_.hasNext()==false);
  }

  void testThrow(TestParams &p) const
  {
    try
    {
      Term::process( string("aaa"), p);             // should throw
      tut::fail("Term::process() didn't failed");
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

factory tf("Persistency/Facades/StrAccess/detail/Term");
} // unnamed namespace


namespace tut
{

// test exception when end is reached
template<>
template<>
void testObj::test<1>(void)
{
  while(!p_.isEnd())
    ++p_;
  testThrow(p_);
}

// test exception when not-last element is set
template<>
template<>
void testObj::test<2>(void)
{
  ++pLast_;
  testThrow(pLast_);
}

// test indirection resolving
template<>
template<>
void testObj::test<3>(void)
{
  int  x =666;
  int *px=&x;
  Term::process(px, pLast_);
  ensure_equals("invalid value returned", cb_.lastValue_, "666");
}

// test collection
template<>
template<>
void testObj::test<4>(void)
{
  vector<int> v(10, 42);
  assert(v.size()==10);
  Term::process(v, pLast_);
  ensure_equals("invalid value returned", cb_.lastSize_, 10);
}

// test indirect collection
template<>
template<>
void testObj::test<5>(void)
{
  vector<int>  v(10, 42);
  assert(v.size()==10);
  vector<int> *pv=&v;
  Term::process(pv, pLast_);
  ensure_equals("invalid value returned", cb_.lastSize_, 10);
}

} // namespace tut
#endif                      
