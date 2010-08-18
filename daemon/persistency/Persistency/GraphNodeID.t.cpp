/*
 * GraphNodeID.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/GraphNodeID.hpp"

using namespace Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/GraphNodeID");
} // unnamed namespace


namespace tut
{

// check setting value in c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const GraphNodeID id(42);
  ensure_equals("invalid ID", id.get(), 42u);
}

// test assignment
template<>
template<>
void testObj::test<2>(void)
{
  const GraphNodeID id1(11);
  GraphNodeID       id2(22);
  id2=id1;
  ensure_equals("ID not assigned", id2.get(), 11u);
}

} // namespace tut
