/*
 * NoInstance.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/NoInstance.hpp"

using namespace System;

namespace
{

struct TestObj: private NoInstance
{
  static int answer(void)
  {
    return 42;
  }
};


struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/NoInstance");
} // unnamed namespace


namespace tut
{

// test accessing some static method
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid answer to ultimate question", TestObj::answer(), 42);
}

} // namespace tut
