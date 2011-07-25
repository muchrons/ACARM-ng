/*
 * NonTerm.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>
#include <boost/mpl/insert.hpp>

#include "Persistency/Facades/StrAccess/detail/NonTerm.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;
#if 0           // TODO                             

namespace
{

struct TestString
{
  TestString(const std::string &str):
    str_(str)
  {
  }

  std::string str_;
}; // struct TestString

struct OnStringTest
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    tut::ensure("terminal element found too soon", p.hasNext());
    typedef typename TParams::template handle<string>::type Handle;
    return Handle::process(e.str_, ++p);
  }
}; // struct OnStringTest

// add std::string support
typedef boost::mpl::insert<DefaultHandleMap, boost::mpl::pair<TestString, OnStringTest>::type >::type StringHandleMap;
// add this to paramter
typedef Params<StringHandleMap,TestParams::ResultCallback> LocalTestParams;


struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_),
    str_("narf"),
    e_(str_)
  {
  }

  LocalTestParams::ResultCallback cb_;
  LocalTestParams                 p_;
  const string                    str_;
  const TestString                e_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/detail/NonTerm");
} // unnamed namespace


namespace tut
{

// test exception if called for last element
template<>
template<>
void testObj::test<1>(void)
{
  ++p_;
  assert(p_.hasNext()==false);
  assert(p_.isEnd()==false);
  try
  {
    NonTerm::process(str_, p_);
    fail("call didn't throw for last element");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test indirect call
template<>
template<>
void testObj::test<2>(void)
{
  NonTerm::process(&e_, p_);
  ensure_equals("invalid response", cb_.lastValue_, str_);
}

// test collection
template<>
template<>
void testObj::test<3>(void)
{
  vector<TestString> vec;
  vec.push_back( TestString("first") );
  vec.push_back(e_);
  vec.push_back( TestString("third") );
  LocalTestParams p(Path("array.1.sth"), cb_);
  NonTerm::process(vec, p);
  ensure_equals("invalid response", cb_.lastValue_, str_);
}

// test collection indirect call
template<>
template<>
void testObj::test<4>(void)
{
  vector<TestString> vec;
  vec.push_back( TestString("first") );
  vec.push_back(e_);
  vec.push_back( TestString("third") );
  LocalTestParams p(Path("array.1.sth"), cb_);
  NonTerm::process(&vec, p);
  ensure_equals("invalid response", cb_.lastValue_, str_);
}

// test if required code is run
template<>
template<>
void testObj::test<5>(void)
{
  NonTerm::process(e_, p_);
  ensure_equals("invalid response", cb_.lastValue_, str_);
}

} // namespace tut
#endif                      
