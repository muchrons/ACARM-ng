/*
 * NodesFifo.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/NodesFifo.hpp"
#include "Core/Types/TestBase.t.hpp"

using namespace Core::Types;

namespace
{

struct TestClass: private TestBase
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/NodesFifo");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  NodesFifo nf;
}

} // namespace tut
