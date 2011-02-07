/*
 * Sources.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "Core/Sources.hpp"

using namespace Core;

namespace
{
struct TestClass
{
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
  Sources s;
}

// test reading from input
template<>
template<>
void testObj::test<2>(void)
{
  Sources                     s;
  Persistency::GraphNodePtrNN leaf=s.read();
  ensure_equals("invalid leaf returned", leaf->getAlert()->getName().get(), std::string("some alert") );
}

} // namespace tut
