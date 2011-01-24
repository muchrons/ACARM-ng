/*
 * LogNULLStream.t.cpp
 *
 */
#include <tut.h>

#include "Logger/detail/LogNULLStream.hpp"

using namespace Logger::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/detail/LogNULLStream");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  LogNULLStream lns;
}

// test appending anything - should compile
template<>
template<>
void testObj::test<2>(void)
{
  LogNULLStream lns;
  lns<<42<<"kszy"<<3.1416;
}

} // namespace tut
