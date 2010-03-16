/*
 * Source.t.cpp
 *
 */
#include <tut/tut.hpp>

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

//
template<>
template<>
void testObj::test<1>(void)
{
  // TODO
}

} // namespace tut
