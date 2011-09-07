/*
 * OnNonTerm.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>
#include <boost/mpl/insert.hpp>

#include "Persistency/Facades/StrAccess/detail/OnNonTerm.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;

namespace
{
string g_toBeCalled;

struct TestString
{
}; // struct TestString

struct OnTestString
{
  template<typename T, typename TParams>
  static bool process(const T &/*e*/, TParams p)
  {
    tut::ensure_equals("unexpected call", "OnTestString", g_toBeCalled);
    tut::ensure("terminal element found too soon", p.hasNext());
    return false;
  }
}; // struct OnTestString

struct OnTestCollectionIndex
{
  template<typename T, typename TParams>
  static bool process(const T &/*e*/, TParams p)
  {
    tut::ensure_equals("unexpected call", "OnTestCollectionIndex", g_toBeCalled);
    tut::ensure_equals("invalid index", p.get(), "666");
    return false;
  }
}; // struct OnTestString

// add std::string support
typedef boost::mpl::map<
                  boost::mpl::pair<CollectionIndexHandle, OnTestCollectionIndex>,
                  boost::mpl::pair<TestString, OnTestString>,
                  boost::mpl::pair<ErrorHandle, ErrorThrower>
                >::type LocalHandleMap;
// add this to paramter
typedef Params<LocalHandleMap, TestParams::ResultCallback> LocalTestParams;


struct TestClass
{
  TestClass(void):
    p_(Path("a.b"), cb_),
    e_()
  {
    g_toBeCalled="?? not set yet ??";
  }

  LocalTestParams::ResultCallback cb_;
  LocalTestParams                 p_;
  const TestString                e_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/detail/OnNonTerm");
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
    OnNonTerm::process(TestString(), p_);
    fail("call didn't throw for last element");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test collection that ends on some index
template<>
template<>
void testObj::test<2>(void)
{
  g_toBeCalled="OnTestCollectionIndex";
  vector<string> vec;
  vec.push_back("kszy");
  LocalTestParams p(Path("array.666"), cb_);
  OnNonTerm::process(vec, p);
}

// test collection that does not end on an index
template<>
template<>
void testObj::test<3>(void)
{
  g_toBeCalled="OnTestCollectionIndex";
  vector<TestString> vec;
  vec.push_back(e_);
  LocalTestParams p(Path("array.666.sth"), cb_);
  OnNonTerm::process(vec, p);
}

} // namespace tut
