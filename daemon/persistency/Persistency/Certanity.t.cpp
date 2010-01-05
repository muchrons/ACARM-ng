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
  const Certanity c(0.10);
  ensure_equals("invalid value", c.get(), 0.10);
}

// test maximum value
template<>
template<>
void testObj::test<2>(void)
{
  const Certanity c(1);
  ensure_equals("invalid value", c.get(), 1);
}

// test default value
template<>
template<>
void testObj::test<3>(void)
{
  Certanity c;
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
  Certanity c(0);
  ensure_equals("invalid value", c.get(), 0);
}

} // namespace tut
