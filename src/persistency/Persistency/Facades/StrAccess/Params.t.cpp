/*
 * Params.t.cpp
 *
 */
#include <tut.h>
#include <boost/type_traits/is_same.hpp>

#include "System/ignore.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;

namespace
{

struct TestHandleMap {};
struct TestCallbacks {};

typedef Params<TestHandleMap, TestCallbacks> TestParams;

struct TestClass
{
  TestClass(void):
    path_("one.two"),
    p_(path_, cb_)
  {
  }

  const Path    path_;
  TestCallbacks cb_;
  TestParams    p_;
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

// test creating from begin/end directly
template<>
template<>
void testObj::test<2>(void)
{
  const TestParams p(path_.begin(), path_.end(), cb_);
  ensure_equals("invalid element", p.get(), "one");
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

// test for validity of handle map declaration
template<>
template<>
void testObj::test<4>(void)
{
  ensure("invalid handle map forward declaration", boost::is_same<TestParams::HandleMap, TestHandleMap>::value);
}

// test for callbacks declaration
template<>
template<>
void testObj::test<5>(void)
{
  TestCallbacks &tmp=p_.callback();
  System::ignore(tmp);
}

// test for end element, when on the end
template<>
template<>
void testObj::test<6>(void)
{
  ++p_;
  ++p_;
  ensure("end not found", p_.isEnd() );
}

} // namespace tut
