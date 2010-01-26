/*
 * OptionalString.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/OptionalString.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
  std::string createImplicit(const OptionalString &os) const
  {
    return os.get();
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/OptionalString");
} // unnamed namespace


namespace tut
{

// check setting string
template<>
template<>
void testObj::test<1>(void)
{
  const OptionalString os("Alice");
  ensure("string not set", os.isSet() );
}

// check not setting string.
template<>
template<>
void testObj::test<2>(void)
{
  const OptionalString os;
  ensure("string set", !os.isSet() );
}

// test creating implicit object from const char* (smoke test)
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid value", createImplicit("DooM"), "DooM");
}

// test getting value when set
template<>
template<>
void testObj::test<4>(void)
{
  const OptionalString str("narf");
  ensure_equals("invalid string set", str.get(), "narf");
}

// test getting value when not set
template<>
template<>
void testObj::test<5>(void)
{
  const OptionalString os;
  try
  {
    os.get();
    fail("get() didn't throw on unset string");
  }
  catch(const OptionalString::ExceptionStringNotSet &)
  {
    // this is expected
  }
}

// test creating implicit object from std::string (smoke test)
template<>
template<>
void testObj::test<6>(void)
{
  const std::string tmp("KszY");
  ensure_equals("invalid value", createImplicit(tmp), tmp);
}

} // namespace tut
