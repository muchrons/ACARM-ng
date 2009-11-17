/*
 * Certanity.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Certanity.hpp"

using namespace Persistency;

namespace
{

struct CertanityTestClass
{
  void check(const double cv)
  {
    try
    {
      const Certanity c(cv);
      tut::fail("Certanity() did'nt throw on invalid value");
    }
    catch(const ExceptionInvalidCertanity&)
    {
      // this is expected
    }
  }
};

typedef CertanityTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Certanity");
} // unnamed namespace


namespace tut
{

// create example certanity value
template<>
template<>
void testObj::test<1>(void)
{
  const Certanity c(10);
  ensure_equals("invalid value", c.get(), 10);
}

// test maximum value
template<>
template<>
void testObj::test<2>(void)
{
  const Certanity c(100);
  ensure_equals("invalid value", c.get(), 100);
}

// test default value
template<>
template<>
void testObj::test<3>(void)
{
  Certanity c;
  ensure_equals("invalid value", c.get(), 100);
}

// check v>100
template<>
template<>
void testObj::test<4>(void)
{
  check(100.1);
}

// check negative value
template<>
template<>
void testObj::test<5>(void)
{
  check(-10);
}

// check zero
template<>
template<>
void testObj::test<6>(void)
{
  check(0);
}

} // namespace tut
