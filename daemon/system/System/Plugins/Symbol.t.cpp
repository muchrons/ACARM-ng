/*
 * Symbol.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/Plugins/Symbol.hpp"
#include "System/Plugins/TestBase.t.hpp"

using namespace System::Plugins;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Plugins/Symbol");
} // unnamed namespace


namespace tut
{

namespace
{
int func(int)
{
  return 42;
}
} // unnamed namespace

// test creating some symbol
template<>
template<>
void testObj::test<1>(void)
{
  Symbol<int(*)(int)> s(h_, "f", func);
  ensure("NULL symbol", s.get()!=NULL );
  ensure_equals("invalid symbol", (*s.get())(666), 42);
}

// try creating symbol of a variable
template<>
template<>
void testObj::test<2>(void)
{
  int x=42;
  Symbol<int*> s(h_, "abc", &x);
  ensure("NULL symbol", s.get()!=NULL );
  ensure_equals("invalid symbol", *s.get(), 42);
}

// try getting name of the symbol
template<>
template<>
void testObj::test<3>(void)
{
  int x=42;
  Symbol<int*> s(h_, "abc", &x);
  ensure_equals("invalid name", s.name(), "abc");
}

// try getting const-symbol
template<>
template<>
void testObj::test<4>(void)
{
  int x=42;
  const Symbol<int*> s(h_, "abc", &x);
  ensure("NULL symbol", s.get()!=NULL );
  ensure_equals("invalid symbol", *s.get(), 42);
}

// test accessing object with * operator
template<>
template<>
void testObj::test<5>(void)
{
  int x=42;
  Symbol<int*> s(h_, "abc", &x);
  ensure("NULL symbol", s.get()!=NULL );
  ensure_equals("invalid name", *s, 42);
}

// test accessing object with * operator - const version
template<>
template<>
void testObj::test<6>(void)
{
  int x=42;
  const Symbol<int*> s(h_, "abc", &x);
  ensure("NULL symbol", s.get()!=NULL );
  ensure_equals("invalid symbol", *s, 42);
}

} // namespace tut
