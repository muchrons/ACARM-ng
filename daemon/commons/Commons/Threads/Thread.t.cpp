/*
 * Thread.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Threads/Thread.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace std;
using namespace Commons::Threads;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Threads/Thread");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

} // namespace tut
