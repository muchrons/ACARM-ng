/*
 * HandleIndirection.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "DataFacades/StrAccess/HandleIndirection.hpp"
#include "DataFacades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace DataFacades::StrAccess;

namespace
{
unsigned int g_MyTestProcessFuncObj_runCnt;

struct MyTestProcessFuncObj
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    ++g_MyTestProcessFuncObj_runCnt;
    BOOST_STATIC_ASSERT( (boost::is_same<T, int>::value) );
    tut::ensure_equals("invalid path", p.path().get(), "a.b");
    tut::ensure_equals("invalid value", e, 42);
    return true;
  }
};

struct TestClass
{
  TestClass(void):
    v_(42),
    p_(Path("a.b"), cb_)
  {
    g_MyTestProcessFuncObj_runCnt=0;
  }

  const int                  v_;
  TestParams::ResultCallback cb_;
  TestParams                 p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/HandleIndirection");
} // unnamed namespace


namespace tut
{

// test non-pointer on funciton object
template<>
template<>
void testObj::test<1>(void)
{
  HandleIndirection::process<MyTestProcessFuncObj>(v_, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test pointer
template<>
template<>
void testObj::test<2>(void)
{
  const int *ptr=&v_;
  HandleIndirection::process<MyTestProcessFuncObj>(ptr, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test smart pointer
template<>
template<>
void testObj::test<3>(void)
{
  boost::shared_ptr<int> ptr(new int(v_));
  HandleIndirection::process<MyTestProcessFuncObj>(ptr, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test pointer to pointer
template<>
template<>
void testObj::test<4>(void)
{
  const int  *ptr =&v_;
  const int **ptr2=&ptr;
  HandleIndirection::process<MyTestProcessFuncObj>(ptr2, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test pointer to smart pointer to pointer
template<>
template<>
void testObj::test<5>(void)
{
  boost::shared_ptr<const int*>  ptr(new const int*(&v_));
  boost::shared_ptr<const int*> *ptr2=&ptr;
  HandleIndirection::process<MyTestProcessFuncObj>(ptr2, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test NULL pointer
template<>
template<>
void testObj::test<6>(void)
{
  const int *ptr=NULL;
  HandleIndirection::process<MyTestProcessFuncObj>(ptr, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 0);
  ensure_equals("null callback not called", cb_.lastNullFound_, "a");
}

// test NULL in smart pointer
template<>
template<>
void testObj::test<7>(void)
{
  boost::shared_ptr<int>  ptr;
  HandleIndirection::process<MyTestProcessFuncObj>(ptr, p_);
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 0);
  ensure_equals("null callback not called", cb_.lastNullFound_, "a");
}

// test no exception when requested indirection translation on last element
template<>
template<>
void testObj::test<8>(void)
{
  while(p_.hasNext())
    ++p_;
  assert(!p_.hasNext());
  HandleIndirection::process<MyTestProcessFuncObj>(&v_, p_);    // must not throw
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 1);
}

// test exception when requested indirection translation on end()
template<>
template<>
void testObj::test<9>(void)
{
  while(!p_.isEnd())
    ++p_;
  assert(p_.isEnd());
  try
  {
    HandleIndirection::process<MyTestProcessFuncObj>(&v_, p_);
    fail("call didn't throw on end of path");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
  ensure_equals("invalid run count", g_MyTestProcessFuncObj_runCnt, 0);
}

namespace
{
struct TestStringObjectStub
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams p)
  {
    tut::ensure_equals("invalid path", p.path().get(), "a.b");
    tut::ensure_equals("invalid value", e, string("some string"));
    return p.callback().value(e);
  }
}; // struct TestStringObjectStub
} // unnamed namespace

// test if indirection handle does NOT break const char*
template<>
template<>
void testObj::test<10>(void)
{
  while(p_.hasNext())
    ++p_;
  const char *ptr="some string";
  HandleIndirection::process<TestStringObjectStub>(ptr, p_);
  ensure_equals("invalid value read", cb_.lastValue_, ptr);
}


namespace
{
struct TestNULLStub
{
  template<typename T, typename TParams>
  static bool process(const T &/*e*/, TParams /*p*/)
  {
    fail("call with NULL paramter has been forwarded");
    return false;
  }
}; // struct TestStringObjectStub
} // unnamed namespace

// test if indirection handles NULLs correctly
template<>
template<>
void testObj::test<11>(void)
{
  const char *ptr=NULL;
  HandleIndirection::process<TestNULLStub>(ptr, p_);
  ensure_equals("invalid NULL location set", cb_.lastNullFound_, "a");
}

// test if indirection handle does NOT break char[]
template<>
template<>
void testObj::test<12>(void)
{
  while(p_.hasNext())
    ++p_;
  char tab[]="some string";
  HandleIndirection::process<TestStringObjectStub>(tab, p_);
}

} // namespace tut
