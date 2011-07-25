/*
 * Params.t.cpp
 *
 */
#include <tut.h>
#include <boost/type_traits/is_same.hpp>

#include "System/ignore.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;
#if 0           // TODO             

namespace
{
struct TestClass
{
  TestClass(void):
    path_("one.two"),
    p_(path_, cb_)
  {
  }

  const Path                 path_;
  TestParams::ResultCallback cb_;
  TestParams                 p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/Params");
} // unnamed namespace


namespace tut
{

// test for end element, when not on the end
template<>
template<>
void testObj::test<1>(void)
{
  ensure("starting with end", !p_.isEnd() );
  ++p_;
  ensure("end after first element", !p_.isEnd() );
}

// test for end element, when on the end
template<>
template<>
void testObj::test<2>(void)
{
  ++p_;
  ++p_;
  ensure("end not found", p_.isEnd() );
}

// test each element's value
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid element 1", p_.get(), "one");
  ++p_;
  ensure_equals("invalid element 2", p_.get(), "two");
}

// test for callbacks declaration
template<>
template<>
void testObj::test<4>(void)
{
  TestCallback &tmp=p_.callback();
  System::ignore(tmp);
}

// test getting full path
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid path", p_.path().get(), path_.get());
}

// test for having next element when there is one
template<>
template<>
void testObj::test<6>(void)
{
  ensure("no next element reported", p_.hasNext());
}

// test for having next element when there is no
template<>
template<>
void testObj::test<7>(void)
{
  ++p_;
  ensure("end came too soon", p_.isEnd()==false);
  ensure("no next element reported", p_.hasNext()==false);
}

// test if hasNext() returns false when end() is reached.
template<>
template<>
void testObj::test<8>(void)
{
  while(!p_.isEnd())
    ++p_;
  ensure("hasNext() does not repotr false for end()", p_.hasNext()==false);
}


namespace
{
struct TestString
{
  TestString(const std::string &str):
    str_(str)
  {
  }

  std::string str_;
};

struct OnString: private System::NoInstance
{
  template<typename T, typename TParams>
  static bool process(const T &e, TParams &p)
  {
    ensure("term is too soon", p.hasNext());
    typedef typename TParams::template handle<string>::type Handle;
    return Handle::process(e.str_, ++p);
  }
}; // struct OnString

typedef boost::mpl::map<
      boost::mpl::pair<TestString, OnString>,
      boost::mpl::pair<ErrorTests, ErrorHandle>
          > TestHandleMap;

typedef Params<TestHandleMap, TestParams::ResultCallback> HandleTestParams;
} // unnamed namespace

// test handle<> when handle exists
template<>
template<>
void testObj::test<9>(void)
{
  HandleTestParams htp(path_, cb_);
  typedef HandleTestParams::handle<TestString>::type Handle;
  ensure("invalid return value from handle", Handle::process(TestString("narf"), htp)==true );
  ensure_equals("invalid string passed", cb_.lastValue_, "narf");
}


namespace
{
struct UnknownKey {};
} // unnamed namespace

// test if code with invalid (unhandled) element in path compiles and throws in runtime
template<>
template<>
void testObj::test<10>(void)
{
  typedef TestParams::handle<UnknownKey>::type Handle;
  try
  {
    Handle::process("abc", p_);
    fail("handle for unknown type didn't throw when called");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

} // namespace tut
#endif              
