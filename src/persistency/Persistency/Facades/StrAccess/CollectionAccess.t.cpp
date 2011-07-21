/*
 * CollectionAccess.t.cpp
 *
 */
#include <tut.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "Persistency/Facades/StrAccess/CollectionAccess.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency::Facades::StrAccess;

namespace
{
  /*
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
*/

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

factory tf("Persistency/Facades/StrAccess/CollectionAccess");
} // unnamed namespace


namespace tut
{
// TODO

// 
template<>
template<>
void testObj::test<1>(void)
{
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

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

} // namespace tut
