/*
 * Alert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/Alert.hpp"

using namespace std;
using namespace Persistency;

// TODO: write tests

namespace
{

struct AlertTestClass
{
  AlertTestClass(void)
  {
  }
};

typedef AlertTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Alert");
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

// 
template<>
template<>
void testObj::test<7>(void)
{
}

} // namespace tut
