/*
 * AccountConfig.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/AccountConfig.hpp"

using namespace std;
using namespace Trigger::GG;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/AccountConfig");
} // unnamed namespace


namespace tut
{

// test getting user's ID
template<>
template<>
void testObj::test<1>(void)
{
  const AccountConfig ac(42, "TheAnswer");
  ensure_equals("invalid user's ID", ac.getUserID(), 42u);
}

// test getting password
template<>
template<>
void testObj::test<2>(void)
{
  const AccountConfig ac(42, "TheAnswer");
  ensure_equals("invalid password", ac.getPassword(), "TheAnswer");
}

// test copyability
template<>
template<>
void testObj::test<3>(void)
{
  const AccountConfig ac1(42, "TheAnswer");
  const AccountConfig ac2(ac1);
  ensure_equals("ID does not match", ac2.getUserID(), ac1.getUserID() );
  ensure_equals("password does not match", ac2.getPassword(), ac1.getPassword() );
}

} // namespace tut
