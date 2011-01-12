/*
 * SignedNodesFifo.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/SignedNodesFifo.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/SignedNodesFifo");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  SignedNodesFifo snf;
}

} // namespace tut
