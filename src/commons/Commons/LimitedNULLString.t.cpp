/*
 * LimitedNULLString.t.cpp
 *
 */
#include <tut.h>

#include "Commons/LimitedNULLString.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Commons;

namespace
{

struct LimitedNULLStringTestClass
{
};

typedef LimitedNULLStringTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/LimitedNULLString");
} // unnamed namespace


namespace tut
{

// check normal configuration
template<>
template<>
void testObj::test<1>(void)
{
  const LimitedNULLString<10> ls("Alice");
  ensure_equals("invaid string saved", ls.get(), string("Alice") );
}

// test NULL
template<>
template<>
void testObj::test<2>(void)
{
  const LimitedNULLString<10> ls(NULL);
  ensure("unable to stroe NULL", ls.get()==NULL);
}

// test too large string
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    LimitedNULLString<10> ls("0123456789-");
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
  const LimitedNULLString<10> ls("0123456789");
  ensure_equals("string has been truncated", strlen( ls.get() ), 10u);
}

// test operator[]
template<>
template<>
void testObj::test<5>(void)
{
  const LimitedNULLString<10> ls("narf");
  ensure_equals("invalid char read at[0]", ls[0],  'n');
  ensure_equals("invalid char read at[4]", ls[4], '\0');
}

// test copy c-tor from string
template<>
template<>
void testObj::test<6>(void)
{
  const LimitedNULLString<10> ls1("narf");
  const LimitedNULLString<10> ls2(ls1);
  ensure("pointers are invalid", ls1.get()!=ls2.get() );
  ensure_equals("invalid string", ls2.get(), string( ls1.get() ) );
}

// test copy c-tor from NULL
template<>
template<>
void testObj::test<7>(void)
{
  const LimitedNULLString<10> ls1(NULL);
  const LimitedNULLString<10> ls2(ls1);
  ensure("pointers are invalid", ls1.get()==ls2.get() );
  ensure("invalid string", ls2.get()==NULL);
}

// test assignment operator from string
template<>
template<>
void testObj::test<8>(void)
{
  const LimitedNULLString<10> ls1("narf");
  LimitedNULLString<10>       ls2("aaa");
  ls2=ls1;
  ensure("pointers are invalid", ls1.get()!=ls2.get() );
  ensure_equals("invalid string", ls2.get(), string( ls1.get() ) );
}

// test assignemtn operator from NULL
template<>
template<>
void testObj::test<9>(void)
{
  const LimitedNULLString<10> ls1(NULL);
  LimitedNULLString<10>       ls2(ls1);
  ls2=ls1;
  ensure("pointers are invalid", ls1.get()==ls2.get() );
  ensure("invalid string", ls2.get()==NULL);
}

// test self copy
template<>
template<>
void testObj::test<10>(void)
{
  LimitedNULLString<10> ls("abc");
  ls=ls;
  ensure("got NULL pointer", ls.get()!=NULL);
  ensure_equals("invalid string", ls.get(), string("abc") );
}

// test creating from string
template<>
template<>
void testObj::test<11>(void)
{
  const string          str("kszy");
  LimitedNULLString<10> ls(str);
  ensure("got NULL pointer", ls.get()!=NULL);
  ensure("invalid pointer assigned", ls.get()!=str.c_str() );
  ensure_equals("invalid string", ls.get(), str);
}

// test comparing non-null strings
template<>
template<>
void testObj::test<12>(void)
{
  const LimitedNULLString<10> ls1 ("ABC");
  const LimitedNULLString<10> ls1o("ABC");
  const LimitedNULLString<10> ls2 ("XYZ");
  TestHelpers::checkEquality(ls1, ls1o, ls2);
}

// test comparing null strings
template<>
template<>
void testObj::test<13>(void)
{
  const LimitedNULLString<10> ls1(NULL);
  const LimitedNULLString<10> ls2(NULL);
  ensure("NULL stirngs differ", ls1==ls2);
}

// test comparing string with NULL string
template<>
template<>
void testObj::test<14>(void)
{
  const LimitedNULLString<10> ls1 ("ABC");
  const LimitedNULLString<10> ls1o("ABC");
  const LimitedNULLString<10> ls2 (NULL);
  TestHelpers::checkEquality(ls1, ls1o, ls2);
}

// test comparing NULL with string
template<>
template<>
void testObj::test<15>(void)
{
  const LimitedNULLString<10> ls1 (NULL);
  const LimitedNULLString<10> ls1o(NULL);
  const LimitedNULLString<10> ls2 ("ABC");
  TestHelpers::checkEquality(ls1, ls1o, ls2);
}

// test default c-tor
template<>
template<>
void testObj::test<16>(void)
{
  const LimitedNULLString<10> tmp;
  ensure_equals("invalid default c-tor", tmp.get(), string("") );
}

// test size export
template<>
template<>
void testObj::test<17>(void)
{
  // NOTE: static cast is required for Intel - dunno why...
  ensure_equals("invalid size", static_cast<int>(LimitedNULLString<42>::size), 42);
}

} // namespace tut
