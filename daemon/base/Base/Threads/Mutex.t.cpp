/*
 * Mutex.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/Mutex.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/Mutex");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  Mutex m;
}

} // namespace tut
