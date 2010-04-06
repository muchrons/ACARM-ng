/*
 * LimitedString.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/detail/LimitedString.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::detail;

namespace
{

struct LimitedStringTestClass
{
};

typedef LimitedStringTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/detail/LimitedString");
} // unnamed namespace


namespace tut
{

// check normal configuration
template<>
template<>
void testObj::test<1>(void)
{
  const LimitedString<10> ls("Alice");
  ensure_equals("invaid string saved", ls.get(), string("Alice") );
}

// test throw on NULL
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    LimitedString<10> ls(NULL);
    fail("LimitedString() didn't throw on NULL");
  }
  catch(const ExceptionNULLParameter&)
  {
    // this is expected
  }
}

// test too large string
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    LimitedString<10> ls("0123456789-");
    fail("LimitedString() didn't throw on too logn string");
  }
  catch(const ExceptionStringTooLong&)
  {
    // this is expected
  }
}

// test equaly long string
template<>
template<>
void testObj::test<4>(void)
{
  const LimitedString<10> ls("0123456789");
  ensure_equals("string has been truncated", strlen( ls.get() ), 10u);
}

// test operator[]
template<>
template<>
void testObj::test<5>(void)
{
  const LimitedString<10> ls("abcdef");
  ensure_equals("invalid char read at[0]", ls[0],  'a');
  ensure_equals("invalid char read at[5]", ls[5],  'f');
  ensure_equals("invalid char read at[6]", ls[6], '\0');
}

// test c-tor form string
template<>
template<>
void testObj::test<6>(void)
{
  const string            str("(: narf :)");
  const LimitedString<10> ls(str);
  ensure_equals("invalid stirng", ls.get(), str);
}

// test equality operators
template<>
template<>
void testObj::test<7>(void)
{
  const LimitedString<10> ls1("kszy");
  const LimitedString<10> ls2("narf");
  TestHelpers::checkEquality(ls1, ls2);
}

// test default c-tor
template<>
template<>
void testObj::test<8>(void)
{
  const LimitedString<10> tmp;
  ensure_equals("invalid value after default c-tor", tmp.get(), string("") );
}

} // namespace tut
