/*
 * ObjectID.t.cpp
 *
 */
#include <tut.h>

#include "Base/ObjectID.hpp"

using namespace Base;

namespace
{

struct TestClass
{
  typedef ObjectID<TestClass> TestID;

  TestClass(void):
    a_(11),
    b_(13)
  {
  }

  const TestID a_;
  const TestID b_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ObjectID");
} // unnamed namespace


namespace tut
{

// tech creating instance
template<>
template<>
void testObj::test<1>(void)
{
  const TestID id(42u);
  ensure_equals("invalid value", id.get(), 42u);
}

// ensure ID is reasonable large
template<>
template<>
void testObj::test<2>(void)
{
  ensure("sizeof of numeric type for ID is invalid", sizeof(TestID::Numeric)==64/8 );
}

// test less-than operator
template<>
template<>
void testObj::test<3>(void)
{
  ensure("a<b failed",     a_<b_  );
  ensure("! a<b failed", !(b_<a_) );
}

// test greater-than
template<>
template<>
void testObj::test<4>(void)
{
  ensure("a>b failed", b_>a_);
}

// test equality
template<>
template<>
void testObj::test<5>(void)
{
  ensure("a==a failed", a_==a_);
}

// test inequality
template<>
template<>
void testObj::test<6>(void)
{
  ensure("a!=b failed", a_!=b_);
}

// test copyability
template<>
template<>
void testObj::test<7>(void)
{
  const TestID id(a_);
  ensure("copy failed", id==a_);
}

// test assignment
template<>
template<>
void testObj::test<8>(void)
{
  TestID id(b_);
  id=a_;
  ensure("assignment failed", id==a_);
}

} // namespace tut
