/*
 * Sources.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "Core/Sources.hpp"
#include "Core/Types/SignedNodesFifo.hpp"

using namespace Core;

namespace
{
struct TestClass
{
  TestClass():
  pl(queue)
  {
  }

  Core::Types::SignedNodesFifo queue;
  MemoryUseChecker pl;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/Sources");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Sources s(&pl);
}

// test reading from input
template<>
template<>
void testObj::test<2>(void)
{
  Sources                     s(&pl);
  Persistency::GraphNodePtrNN leaf=s.read();
  ensure_equals("invalid leaf returned", leaf->getAlert()->getName().get(), std::string("some alert") );
}

} // namespace tut
