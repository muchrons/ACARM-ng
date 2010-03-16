/*
 * Source.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "Core/Source.hpp"

using namespace Core;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/Source");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Source s;
}

// test reading from input
template<>
template<>
void testObj::test<2>(void)
{
  Source                      s;
  Persistency::GraphNodePtrNN leaf=s.read();
  ensure_equals("invalid leaf returned",
                leaf->getAlert()->getName().get(),
                std::string("some alert") );
}

} // namespace tut
