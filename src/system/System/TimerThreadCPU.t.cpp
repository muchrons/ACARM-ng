/*
 * TimerThreadCPU.t.cpp
 *
 */
#include <tut.h>

#include "System/TimerThreadCPU.hpp"

using namespace System;

namespace
{
struct TestClass
{
  TimerThreadCPU t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/TimerThreadCPU");
}

namespace tut
{

// test if resolution is positive
template<>
template<>
void testObj::test<1>(void)
{
  ensure("resolution is not a positive number", t_.resolution()>0.0);
}

} // namespace tut
