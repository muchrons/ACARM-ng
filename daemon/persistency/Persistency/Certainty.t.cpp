/*
 * Certainty.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Certainty.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace Persistency;

namespace
{

struct CertaintyTestClass
{
  void check(const double cv)
  {
    try
    {
      const Certainty c(cv);
      tut::fail("Certainty() did'nt throw on invalid value");
    }
    catch(const ExceptionInvalidCertainty&)
    {
      // this is expected
    }
  }
};

typedef CertaintyTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Certainty");
} // unnamed namespace


namespace tut
{

// create example certanity value
template<>
template<>
void testObj::test<1>(void)
{
  const Certainty c(0.10);
  ensure_equals("invalid value", c.get(), 0.10);
}

// test maximum value
template<>
template<>
void testObj::test<2>(void)
{
  const Certainty c(1);
  ensure_equals("invalid value", c.get(), 1);
}

// test default value
template<>
template<>
void testObj::test<3>(void)
{
  const Certainty c;
  ensure_equals("invalid value", c.get(), 1);
}

// check v>1
template<>
template<>
void testObj::test<4>(void)
{
  check(1.001);
}

// check negative value
template<>
template<>
void testObj::test<5>(void)
{
  check(-0.1);
}

// check zero
template<>
template<>
void testObj::test<6>(void)
{
  const Certainty c(0);
  ensure_equals("invalid value", c.get(), 0);
}

// check == and !=
template<>
template<>
void testObj::test<7>(void)
{
  const Certainty c1(0.1);
  const Certainty c2(0.2);
  TestHelpers::checkEquality(c1, c2);
}

} // namespace tut
