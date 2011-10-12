/*
 * OnCollectionIndex.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <string>
#include <boost/mpl/insert.hpp>

#include "DataFacades/StrAccess/OnCollectionIndex.hpp"
#include "DataFacades/StrAccess/TestParams.t.hpp"

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

factory tf("Persistency/Facades/StrAccess/OnCollectionIndex");
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
  OnCollectionIndex::process(v_, p);
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
  OnCollectionIndex::process(v_, p);
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
    OnCollectionIndex::process(v_, p);
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
    OnCollectionIndex::process(v_, p);
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
  OnCollectionIndex::process(v_, p);
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
}; // struct TestForwardElement

struct OnTestForwardElement
{
  template<typename TParams>
  static bool process(const TestForwardElement &e, TParams p)
  {
    ensure("end has come too soon", p.hasNext());
    return MainDispatcher::process(e.str_, ++p);
  }
}; // struct OnTestForwardElement

typedef boost::mpl::insert<TestParams::HandleMap,
                            boost::mpl::pair<TestForwardElement,OnTestForwardElement>::type
                          >::type TestForwardHandleMap;

typedef Params<TestForwardHandleMap, TestParams::ResultCallback> TestForwardParams;
} // unnamed namespace

// test when index is non-terminal but the other element is
template<>
template<>
void testObj::test<6>(void)
{
  vector<TestForwardElement> v;
  v.push_back( TestForwardElement("hello evil") );
  TestForwardParams p(Path("0.str"), cb_);
  OnCollectionIndex::process(v, p);
  ensure_equals("invalid result", cb_.lastValue_, "hello evil");
}


namespace
{
struct TestForwardElement2
{
  TestForwardElement2(const TestForwardElement &tfe):
    tfe_(tfe)
  {
  }

  TestForwardElement tfe_;
}; // struct TestForwardElement2

struct OnTestForwardElement2
{
  template<typename TParams>
  static bool process(const TestForwardElement2 &e, TParams p)
  {
    ensure("end has come too soon", p.hasNext());
    return MainDispatcher::process(e.tfe_, ++p);
  }
}; // struct OnTestForwardElement2

typedef boost::mpl::insert<TestForwardHandleMap,
                            boost::mpl::pair<TestForwardElement2,OnTestForwardElement2>::type
                          >::type TestForwardHandleMap2;

typedef Params<TestForwardHandleMap2, TestParams::ResultCallback> TestForwardParams2;
} // unnamed namespace

// test when index is non-terminal and the next element is not as well
template<>
template<>
void testObj::test<7>(void)
{
  vector<TestForwardElement2> v;
  v.push_back( TestForwardElement2( TestForwardElement("hello evil") ) );
  TestForwardParams2 p(Path("0.tfe.str"), cb_);
  OnCollectionIndex::process(v, p);
  ensure_equals("invalid result", cb_.lastValue_, "hello evil");
}

// test colleciton of collections
template<>
template<>
void testObj::test<8>(void)
{
  vector<TestForwardElement>           v;
  vector< vector<TestForwardElement> > vv;
  vv.push_back(v);
  v.push_back( TestForwardElement("hello evil") );
  vv.push_back(v);
  TestForwardParams p(Path("1.0.str"), cb_);
  OnCollectionIndex::process(vv, p);
  ensure_equals("invalid result", cb_.lastValue_, "hello evil");
}

// test index out of bound
template<>
template<>
void testObj::test<9>(void)
{
  vector<string> v;
  v.push_back("hello evil");
  TestForwardParams p(Path("x.1"), cb_);
  ++p;
  try
  {
    OnCollectionIndex::process(v, p);
    fail("index out of bound has not been signalled");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test query for collection's size
template<>
template<>
void testObj::test<10>(void)
{
  vector<TestForwardElement> v;
  v.push_back( TestForwardElement("hello evil 3") );
  v.push_back( TestForwardElement("hello evil 2") );
  v.push_back( TestForwardElement("hello evil 1") );
  TestForwardParams p(Path("coll.size"), cb_);
  ++p;
  OnCollectionIndex::process(v, p);
  ensure_equals("invalid result", cb_.lastSize_, 3);
}

// test if path after 'size' does not exist
template<>
template<>
void testObj::test<11>(void)
{
  vector<TestForwardElement> v;
  TestForwardParams p(Path("size.error"), cb_);
  try
  {
    OnCollectionIndex::process(v, p);
    fail("processing didn't throw on extra tokens");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

} // namespace tut
