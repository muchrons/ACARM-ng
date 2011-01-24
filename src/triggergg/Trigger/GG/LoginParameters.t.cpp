/*
 * LoginParameters.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/LoginParameters.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_(42, "abc")
  {
  }

  const AccountConfig cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/LoginParameters");
} // unnamed namespace


namespace tut
{

// test get() const
template<>
template<>
void testObj::test<1>(void)
{
  const LoginParameters lp(cfg_);
  ensure_equals("unvalid user's ID", lp.get().uin, 42);
}

// test get()
template<>
template<>
void testObj::test<2>(void)
{
  LoginParameters lp(cfg_);
  ensure_equals("unvalid user's ID", lp.get().uin, 42);
}

// test password
template<>
template<>
void testObj::test<3>(void)
{
  const LoginParameters lp(cfg_);
  ensure_equals("invalid password", lp.get().password, std::string("abc") );
}

} // namespace tut
