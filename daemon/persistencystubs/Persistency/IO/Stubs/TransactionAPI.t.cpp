/*
 * TransactionAPI.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Stubs/TransactionAPI.hpp"

using namespace Persistency::Stubs;

namespace
{
struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Stubs/TransactionAPI");
} // unnamed namespace


namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  TransactionAPI tapi;
}

} // namespace tut
