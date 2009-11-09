/*
 * Value.t.cpp
 *
 */
#include <tut.h>

#include "XML/Value.hpp"


namespace XML
{

struct ValueTestClass
{
};

} // namespace XML


namespace tut
{
typedef XML::ValueTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/Value");
}

using namespace std;
using namespace XML;


namespace tut
{

// try constructing value from a string
template<>
template<>
void testObj::test<1>(void)
{
  const string test("something");
  const Value  v(test);
  ensure("stored value does not mach set one", v.get()==test);
}

// try constructing from const char*.
template<>
template<>
void testObj::test<2>(void)
{
  const char  *test="something";
  const Value  v(test);
  ensure("stored value does not mach set one", v.get()==test);
}

// test equality operator.
template<>
template<>
void testObj::test<3>(void)
{
  const char  *test="alice has a cat";
  const Value  v(test);
  ensure("equality failed for the same elements for Value",
         v==v);
  ensure("equality returned true for non-equal elements",
         !(v==Value("and now something compleatly different")) );
}

// test inequality operator.
template<>
template<>
void testObj::test<4>(void)
{
  const char  *test="alice has a cat";
  const Value  v(test);
  ensure("inequality failed for the same elements for char*",
         !(v!=v) );
  ensure("inequality returned false for non-equal elements",
         v!=Value("and now something compleatly different") );
}

// test default constructor.
template<>
template<>
void testObj::test<5>(void)
{
  Value v;
  ensure("default constructor created non-empty value", v.get()=="");
}

} // namespace tut
