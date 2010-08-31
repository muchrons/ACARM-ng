/*
 * PortNumber.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/PortNumber.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/PortNumber");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const PortNumber pn(42);
  ensure_equals("invalid port", pn.get(), 42u);
}

// test throw on invalid value
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    PortNumber pn(0);
    fail("c-tor didn't throw on invalid port number");
  }
  catch(const PortNumber::ExceptionInvalidPort &)
  {
    // this is expected
  }
}


namespace
{
void makeNumber(const PortNumber &pn)
{
  ensure_equals("invalid port", pn.get(), 123u);
} // makeNumber()
} // unnamed namespace

// test implicit creation
template<>
template<>
void testObj::test<3>(void)
{
  makeNumber(123);
}

// test copyability
template<>
template<>
void testObj::test<4>(void)
{
  const PortNumber pn1(1);
  PortNumber       pn2(2);
  pn2=pn1;
  ensure_equals("invalid port number", pn2.get(), 1u);
}

// test equality
template<>
template<>
void testObj::test<5>(void)
{
  const PortNumber pn1(1);
  const PortNumber pn2(2);
  ensure("self-comaprison with == fails", pn1==pn1);
  ensure("!(==) fails", !(pn1==pn2) );
}

// test inequality
template<>
template<>
void testObj::test<6>(void)
{
  const PortNumber pn1(1);
  const PortNumber pn2(2);
  ensure("self-comaprison with != does not fail", !(pn1!=pn1) );
  ensure("!= fails", pn1!=pn2);
}

// test less-than
template<>
template<>
void testObj::test<7>(void)
{
  const PortNumber pn1(1);
  const PortNumber pn2(2);
  ensure("less than fails", pn1<pn2);
  ensure("less than doesn't fail for false", !(pn2<pn1) );
}

// test greater-than
template<>
template<>
void testObj::test<8>(void)
{
  const PortNumber pn1(1);
  const PortNumber pn2(2);
  ensure("greater than fails", !(pn1>pn2) );
  ensure("greater than doesn't fail for false", !(pn1>pn2) );
}

// test comparison with implicit conversion - this test must compile
template<>
template<>
void testObj::test<9>(void)
{
  const PortNumber pn(10);
  ensure("comparison iwht instant value fails", pn<42 );
}

} // namespace tut
