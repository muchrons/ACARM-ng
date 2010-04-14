/*
 * ReadWriteMutex.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/ReadWriteMutex.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/ReadWriteMutex");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  ReadWriteMutex m;
}

} // namespace tut
