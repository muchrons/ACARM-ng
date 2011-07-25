/*
 * CollectionIndexHandle.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <string>
#include <boost/mpl/insert.hpp>

#include "Persistency/Facades/StrAccess/CollectionIndexHandle.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void)
  {
    v_.push_back("one");
    v_.push_back("zwei");
    v_.push_back("trzy");
  }

  TestParams::ResultCallback cb_;
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
  TestParams p(Path("v.0"), cb_);
  ++p;
  assert(p.get()=="0");
  CollectionIndexHandle::process(v_, p);
  ensure_equals("invalid result", cb_.lastValue_, "one");
}

// test getting last element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("v.2"), cb_);
  ++p;
  assert(p.get()=="2");
  CollectionIndexHandle::process(v_, p);
  ensure_equals("invalid result", cb_.lastValue_, "trzy");
}

// test index out of bound
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("v.3"), cb_);
  ++p;
  assert(p.get()=="3");
  try
  {
    CollectionIndexHandle::process(v_, p);
    fail("processing didn't throw on invalid index");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test invalid index (ex.: non-number)
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("v.narf"), cb_);
  ++p;
  assert(p.get()=="narf");
  try
  {
    CollectionIndexHandle::process(v_, p);
    fail("processing didn't throw on non-numeric index");
  }
  catch(const Commons::Convert::ExceptionConversionError &)
  {
    // this is expected
  }
}

// test when index is terminal
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("v.1"), cb_);
  ++p;
  assert(p.get()=="1");
  CollectionIndexHandle::process(v_, p);
  ensure_equals("invalid result", cb_.lastValue_, "zwei");
}


namespace
{
struct TestForwardElement
{
  TestForwardElement(const string &str):
    str_(str)
  {
  }

  string str_;
};

struct OnTestForwardElement
{
  template<typename TParams>
  static bool process(const TestForwardElement &e, TParams &p)
  {
    ensure("end has come too soon", p.hasNext());
    return detail::Term::process(e.str_, ++p);
  }
}; // struct TestForwardElement

typedef boost::mpl::insert<TestParams::HandleMap,
                            boost::mpl::pair<TestForwardElement,OnTestForwardElement>::type
                          >::type TestForwardHandleMap;

typedef Params<TestForwardHandleMap, TestParams::ResultCallback> TestForwardParams;
} // unnamed namespace

// test when index is non-terminal and the next element is not as well
template<>
template<>
void testObj::test<6>(void)
{
  /*
  vector<TestForwardElement> v;
  v.push_back( TestForwardElement("hello evil") );
  TestParams p(Path("0.tfe.str"), cb_);
  CollectionIndexHandle::process(v, p);
  ensure_equals("invalid result", cb_.lastValue_, "hello evil");
  */
  // TODO
}

// test when index is non-terminal but the other element is
template<>
template<>
void testObj::test<7>(void)
{
  /*
  TestParams p(Path("v.1"), cb_);
  ++p;
  assert(p.get()=="1");
  CollectionIndexHandle::process(v_, p);
  ensure_equals("invalid result", cb_.lastValue_, "zwei");
  */
// TODO
}

// test colleciton of collections
template<>
template<>
void testObj::test<8>(void)
{
// TODO
}

// test index out of bound
template<>
template<>
void testObj::test<9>(void)
{
// TODO
}

} // namespace tut
