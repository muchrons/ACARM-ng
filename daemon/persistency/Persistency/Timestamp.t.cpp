/*
 * Timestamp.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Timestamp.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Timestamp");
} // unnamed namespace


namespace tut
{

// check default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  Timestamp ts;
}

// check equality operator
template<>
template<>
void testObj::test<2>(void)
{
  const Timestamp ts1;
  const Timestamp ts2=ts1;
  ensure("comparison failed", ts1==ts2);
}

} // namespace tut
