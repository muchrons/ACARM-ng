/*
 * ExceptionInvalidFormat.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <cstring>

#include "Filter/IPBlackList/ExceptionInvalidFormat.hpp"

using namespace std;
using namespace Filter::IPBlackList;

namespace
{

struct TestClass
{
  void testStr(const char *in, const char *out) const
  {
    const char *mark="problem while parsing input near '";
    const ExceptionInvalidFormat e( SYSTEM_SAVE_LOCATION, in, "NaRf");
    const char *tmp=strstr( e.what(), mark);
    tut::ensure("oops - mark not found", tmp!=NULL);
    tmp+=strlen(mark);
    tut::ensure_equals("invalid string", tmp, string(out)+"': NaRf");
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/ExceptionInvalidFormat");
} // unnamed namespace


namespace tut
{

// test normal message
template<>
template<>
void testObj::test<1>(void)
{
  testStr("abc", "abc");
}

// test non-printable characters
template<>
template<>
void testObj::test<2>(void)
{
  testStr("ab\nc", "ab.c");
}

// test too long string
template<>
template<>
void testObj::test<3>(void)
{
  testStr("0123456789a123456789b123456789c123456789d123456789e123456789f123XXX",
          "0123456789a123456789b123456789c123456789d123456789e12345678[...]");
}

// test max-length string
template<>
template<>
void testObj::test<4>(void)
{
  testStr("0123456789a123456789b123456789c123456789d123456789e123456789f123",
          "0123456789a123456789b123456789c123456789d123456789e123456789f123");
}

} // namespace tut
