/*
 * Comparer.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "System/detail/Comparer.hpp"

using namespace std;
using namespace System::detail;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/detail/Comparer");
}


namespace tut
{

// test comapring strings
template<>
template<>
void testObj::test<1>(void)
{
  const bool ret=Comparer<std::string>::lessThan( string("a"), string("b") );
  ensure("invalid less than comparison", ret);
}

// invalid reverted compariosn
template<>
template<>
void testObj::test<2>(void)
{
  const bool ret=Comparer<std::string>::lessThan( string("b"), string("a") );
  ensure("invalid negative less than comparison", !ret);
}

// test comparing const char *
template<>
template<>
void testObj::test<3>(void)
{
  // ensure reverse order in memory
  char str[]="b\0a";
  const char *a=str+2;
  const char *b=str+0;
  ensure("invalid order in memory", b<a);
  ensure_equals("invalid string 'a'", a, string("a") );
  ensure_equals("invalid string 'b'", b, string("b") );
  // compare
  const bool ret=Comparer<const char*>::lessThan(a, b);
  ensure("invalid less than comparison", ret);
}

// test reverse string comparison
template<>
template<>
void testObj::test<4>(void)
{
  // ensure reverse order in memory
  char str[]="b\0a";
  const char *a=str+2;
  const char *b=str+0;
  ensure("invalid order in memory", b<a);
  ensure_equals("invalid string 'a'", a, string("a") );
  ensure_equals("invalid string 'b'", b, string("b") );
  // compare
  const bool ret=Comparer<const char*>::lessThan(b, a);
  ensure("invalid negative less than comparison", !ret);
}

// test char* specialization
template<>
template<>
void testObj::test<5>(void)
{
  char a[]="a";
  char b[]="b";

  const bool ret1=Comparer<char*>::lessThan(a, b);
  const bool ret2=Comparer<const char*>::lessThan(a, b);
  ensure_equals("specialization differs", ret1, ret2);
}

} // namespace tut
