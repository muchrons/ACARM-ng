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
  ensure_equals("invalid port", pn.get(), 123);
} // makeNumber()
} // unnamed namespace

// test implicit creation
template<>
template<>
void testObj::test<3>(void)
{
  makeNumber(123);
}

} // namespace tut
