/*
 * OneInstanceAtOnce.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/OneInstanceAtOnce.hpp"

using namespace System;

namespace
{

struct TestObj: private OneInstanceAtOnce<TestObj, false>
{
};
struct OtherTestObj: private OneInstanceAtOnce<OtherTestObj, false>
{
};


struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/OneInstanceAtOnce");
} // unnamed namespace


namespace tut
{

// test creating single instance
template<>
template<>
void testObj::test<1>(void)
{
  TestObj tmp;
}

// test creating multiple instances one after another
template<>
template<>
void testObj::test<2>(void)
{
  {
    TestObj tmp;
  }
  {
    TestObj tmp;
  }
}

// test creating second instance in release mode
template<>
template<>
void testObj::test<3>(void)
{
  TestObj tmp1;
  try
  {
    TestObj tmp2;
    fail("exception not thrown for the second instance");
  }
  catch(const std::logic_error &)
  {
    // this is expected
  }
}

// test if creating instances of different types does not colide
template<>
template<>
void testObj::test<4>(void)
{
  TestObj      tmp1;
  OtherTestObj tmp2;
}

} // namespace tut
