/*
 * LimitedNULLString.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/detail/LimitedNULLString.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::detail;

namespace
{

struct LimitedNULLStringTestClass
{
};

typedef LimitedNULLStringTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/detail/LimitedNULLString");
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
  ensure_equals("string has been truncated", strlen( ls.get() ), 10);
}

} // namespace tut
