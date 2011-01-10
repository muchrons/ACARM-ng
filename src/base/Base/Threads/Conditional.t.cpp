/*
 * Conditional.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/Conditional.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/Conditional");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  Conditional c;
}

} // namespace tut
