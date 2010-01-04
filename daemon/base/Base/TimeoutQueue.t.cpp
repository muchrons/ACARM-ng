/*
 * TimeoutQueue.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/TimeoutQueue.hpp"

using namespace std;
using namespace tut;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string           Type;
  typedef TimeoutQueue<Type>    Queue;

  typedef Queue::iterator       Iter;
  typedef Queue::const_iterator ConstIter;

  TestClass(void)
  {
  }
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/TimeoutQueue");
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

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

// 
template<>
template<>
void testObj::test<10>(void)
{
}

// 
template<>
template<>
void testObj::test<11>(void)
{
}

// 
template<>
template<>
void testObj::test<12>(void)
{
}

// 
template<>
template<>
void testObj::test<13>(void)
{
}

// 
template<>
template<>
void testObj::test<14>(void)
{
}

// 
template<>
template<>
void testObj::test<15>(void)
{
}

// 
template<>
template<>
void testObj::test<16>(void)
{
}

} // namespace tut
