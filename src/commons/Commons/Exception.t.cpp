/*
 * Exception.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Commons/Exception.hpp"

using namespace Commons;

namespace
{
struct TestException: public Commons::Exception
{
  TestException(void):
    Commons::Exception("ExceptionTestClass::ExceptionTestClass()", "blah")
  {
  }

  const char *ensureString(const char *str)
  {
    return Commons::Exception::ensureString(str);
  }
};

struct ExceptionTestClass
{
  TestException te_;
};

typedef ExceptionTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Exception");
} // unnamed namespace


namespace tut
{

// check ensureString for non-NULL
template<>
template<>
void testObj::test<1>(void)
{
  const char *tmp="alice has a cat";
  ensure("invalid poitner returned", te_.ensureString(tmp)==tmp);
}

// check ensureString for NULL
template<>
template<>
void testObj::test<2>(void)
{
  ensure("NULL pointer returned", te_.ensureString(NULL)!=NULL);
}

// test construction
template<>
template<>
void testObj::test<3>(void)
{
  // this is just a smoke test
  Exception e("where-not-NULL", "narf");
}

} // namespace tut
