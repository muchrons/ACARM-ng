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

struct MyTestProcessFuncObj
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    BOOST_STATIC_ASSERT( (boost::is_same<T, int>::value) );
    tut::ensure_equals("invalid path", p.path().get(), "a.b");
    tut::ensure_equals("invalid value", e, 42);
    return true;
  }
};

struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_)
  {
  }

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
  int v=42;
  HandleIndirection<MyTestProcessFuncObj>::process(v, p_);
}

// test pointer
template<>
template<>
void testObj::test<2>(void)
{
}

// test smart pointer
template<>
template<>
void testObj::test<3>(void)
{
}

// test pointer to pointer
template<>
template<>
void testObj::test<4>(void)
{
}

// test pointer to smart pointer to pointer
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
