/*
 * Base.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <cassert>

#include "System/Exceptions/Base.hpp"

namespace System
{
namespace Exceptions
{

struct BaseTestData
{
};

} // namespace Exceptions
} // namespace System


namespace tut
{
typedef System::Exceptions::BaseTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Exceptions/Base");
}

using namespace std;
using namespace System;
using namespace System::Exceptions;

namespace tut
{

namespace
{
struct MyException: public Base<MyException, std::exception>
{
  template<typename T>
  MyException(const T &t):
    Base<MyException, std::exception>(t)
  {
  }
}; // struct MyException

const char *someStr="hello world!";
} // unnamed namespace

// test creating an instance
template<>
template<>
void testObj::test<1>(void)
{
  const MyException me("hello world!");
}

// test what() method
template<>
template<>
void testObj::test<2>(void)
{
  const MyException me(someStr);
  ensure_equals("invalid string", me.what(), string(someStr) );
}

// test whatAsStr() method
template<>
template<>
void testObj::test<3>(void)
{
  const MyException me(someStr);
  ensure_equals("invalid string", me.whatAsStr(), someStr);
}

// test for base class
template<>
template<>
void testObj::test<4>(void)
{
  const MyException  me(someStr);
  const exception   &base=me;   // this line must compile
  base.what();                  // this suppress warning from compiler
}

} // namespace tut
