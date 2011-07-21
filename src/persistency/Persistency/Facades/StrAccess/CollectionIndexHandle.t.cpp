/*
 * CollectionIndexHandle.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <string>

#include "Persistency/Facades/StrAccess/CollectionIndexHandle.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
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
    v_.push_back("one");
    v_.push_back("zwei");
    v_.push_back("trzy");
  }

  TestParams::ResultCallback cb_;
  TestParams                 p_;
  vector<string>             v_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/CollectionIndexHandle");
} // unnamed namespace


namespace tut
{

// test getting first element
template<>
template<>
void testObj::test<1>(void)
{
  /*
  TestParams p(Path("a.0"), cb_);
  ++p;
  assert(p.get()=="0");
  CollectionIndexHandle::process(v_, p);
  ensure_equals("invalid result", cb_.lastValue_, "one");
  */
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
