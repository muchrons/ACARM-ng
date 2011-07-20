/*
 * DefaultHandleMap.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace boost::mpl;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_)
  {
  }

  TestParams::ResultCallback cb_;
  const TestParams           p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/DefaultHandleMap");
} // unnamed namespace


namespace tut
{

// test term
template<>
template<>
void testObj::test<1>(void)
{
  //const string str=at<DefaultHandleMap,OnTerm>::type::get("test");
  const string str=TestParams::handle<OnTerm>::type::get("test");
  ensure_equals("invalid term handle", str, "test");
}

// test handling of sample path on error
template<>
template<>
void testObj::test<2>(void)
{
  //at<DefaultHandleMap, ErrorTests>::type::throwIfEnd(SYSTEM_SAVE_LOCATION, p_); // thould not throw
  TestParams::handle<ErrorTests>::type::throwIfEnd(SYSTEM_SAVE_LOCATION, p_); // thould not throw
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
