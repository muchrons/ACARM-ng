/*
 * HostRole.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/HostRole.hpp"

using namespace Persistency;

namespace
{

struct HostRoleTestClass
{
};

typedef HostRoleTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/HostRole");
} // unnamed namespace


namespace tut
{

// test creating
template<>
template<>
void testObj::test<1>(void)
{
  HostRole sl(HostRole::SOURCE);
}

// test severityies values
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("source has invalid value", HostRole::SOURCE, 1);
  ensure_equals("target has invalid value", HostRole::TARGET, 2);
}

} // namespace tut
