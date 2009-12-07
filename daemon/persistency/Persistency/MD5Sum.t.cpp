/*
 * MD5Sum.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/MD5Sum.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct MD5SumTestClass
{
  void ensureThrow(const char *md5)
  {
    try
    {
      MD5Sum::createFromString(md5);    // should throw
      tut::fail("MD5Sum::createFromString(): didn't throw on invalid md5");
    }
    catch(const Exception &)
    {
      // this is expected
    }
  }
};

typedef MD5SumTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/MD5Sum");
} // unnamed namespace


namespace tut
{

// check named c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const char *md5="01234567890123456789012345678901";
  MD5Sum      s1 =MD5Sum::createFromString(md5);
  ensure_equals("invlaid md5", s1.get(), string(md5) );
}

// check too long string
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow("01234567890123456789012345678901x");
}

// check too short
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow("0123456789012345678901234567890");
}

// check uppercase
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow("0123456789012345678901234567890V");
}

// test creating form valid string
template<>
template<>
void testObj::test<5>(void)
{
  MD5Sum::createFromString("a1b3c5d7890123456789012345678901");
}

} // namespace tut
