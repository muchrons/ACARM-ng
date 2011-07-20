/*
 * TermCast.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/TermCast.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/TermCast");
} // unnamed namespace


namespace tut
{

// test casting std::string
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid convertion", TermCast::cast( string("abc") ), "abc");
}

// test casting const char *
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid convertion", TermCast::cast("abc"), string("abc") );
}

// test casting from type*
template<>
template<>
void testObj::test<3>(void)
{
  const string str("abc");
  ensure_equals("invalid convertion", TermCast::cast(&str), str);
}

// test casting from const char *=NULL
template<>
template<>
void testObj::test<4>(void)
{
  const char *str=NULL;
  ensure_equals("invalid convertion", TermCast::cast(str), string("<NULL>") );
}

// test casting from const string *=NULL
template<>
template<>
void testObj::test<5>(void)
{
  const string *str=NULL;
  ensure_equals("invalid convertion", TermCast::cast(str), string("<NULL>") );
}

} // namespace tut
