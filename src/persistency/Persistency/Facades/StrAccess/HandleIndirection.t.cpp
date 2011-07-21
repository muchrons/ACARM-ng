/*
 * HandleIndirection.t.cpp
 *
 */
#include <tut.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "Persistency/Facades/StrAccess/HandleIndirection.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency::Facades::StrAccess;

namespace
{
unsigned int g_MyTestProcessFuncObj_runCnt;

struct MyTestProcessFuncObj
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
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

factory tf("Persistency/Facades/StrAccess/HandleIndirection");
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

// test exception when requested indirection translation on end()
template<>
template<>
void testObj::test<8>(void)
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
}

} // namespace tut
