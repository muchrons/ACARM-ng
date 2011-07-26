/*
 * MainDispatcher.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>
#include <boost/mpl/insert.hpp>

#include "Persistency/Facades/StrAccess/detail/MainDispatcher.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;

namespace
{
string g_toBeCalled;

struct SomeTestNonTerm {};

struct OnTestTerm
{
  template<typename T, typename TParams>
  static bool process(const T &/*e*/, TParams &/*p*/)
  {
    tut::ensure_equals("invalid handle for test term called", "OnTestTerm", g_toBeCalled);
    return false;
  }
}; // struct OnTestTerm

struct OnTestNonTerm
{
  template<typename T, typename TParams>
  static bool process(const T &/*e*/, TParams &/*p*/)
  {
    tut::ensure_equals("invalid handle for test non-term called", "OnTestNonTerm", g_toBeCalled);
    return false;
  }
}; // struct OnTestNonTerm


// test map for handling different situations
typedef boost::mpl::map<
              boost::mpl::pair<ErrorTests, ErrorHandle>::type,
              boost::mpl::pair<OnTerm,     OnTestTerm>::type,
              boost::mpl::pair<OnNonTerm,  OnTestNonTerm>::type
            >::type StringHandleMap;

// add this to paramter
typedef Params<StringHandleMap,TestParams::ResultCallback> LocalTestParams;


struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_)
  {
    g_toBeCalled="?? not set ??";
  }

  LocalTestParams::ResultCallback cb_;
  LocalTestParams                 p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/detail/MainDispatcher");
} // unnamed namespace


namespace tut
{

// test non-term
template<>
template<>
void testObj::test<1>(void)
{
  g_toBeCalled="OnTestNonTerm";
  MainDispatcher::process(SomeTestNonTerm(), p_);
}

// test term
template<>
template<>
void testObj::test<2>(void)
{
  g_toBeCalled="OnTestTerm";
  MainDispatcher::process(string("abc"), p_);
}

// test indirection handling
template<>
template<>
void testObj::test<3>(void)
{
  g_toBeCalled="OnTestTerm";
  const string str("abc");
  MainDispatcher::process(&str, p_);
}

// test exception when path ends to soon
template<>
template<>
void testObj::test<4>(void)
{
  g_toBeCalled="OnTestTerm";
  while(!p_.isEnd())
    ++p_;
  try
  {
    MainDispatcher::process("xyz", p_);
    fail("process() didn't throw on end of path");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test collection that is a term
template<>
template<>
void testObj::test<5>(void)
{
  while(p_.hasNext())
    ++p_;
  g_toBeCalled="OnTestTerm";
  vector<string> v;
  v.push_back("abc");
  MainDispatcher::process(v, p_);
}

// test collection that is a non-term
template<>
template<>
void testObj::test<6>(void)
{
  g_toBeCalled="OnTestNonTerm";
  vector<string> v;
  v.push_back("abc");
  MainDispatcher::process(v, p_);
}

} // namespace tut
