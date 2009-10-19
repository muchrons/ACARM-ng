/*
 * BadAlloc.t.cpp
 *
 */
#include <tut.h>

#include "System/Exceptions/BadAlloc.hpp"

namespace System
{
namespace Exceptions
{

struct BadAllocTestData
{
};

} // namespace Exceptions
} // namespace System


namespace tut
{
typedef System::Exceptions::BadAllocTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Exceptions/BadAlloc");
}

using namespace std;
using namespace System::Exceptions;

namespace tut
{

namespace
{
struct MyException: public BadAlloc<MyException>
{
  template<typename T>
  MyException(const T &t):
    BadAlloc<MyException>(t)
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
  const MyException me(someStr);
}

// test for base class
template<>
template<>
void testObj::test<2>(void)
{
  const MyException  me(someStr);
  const bad_alloc   &base=me;   // this line must compile
  base.what();                  // this suppress warning from compiler
}

} // namespace tut

