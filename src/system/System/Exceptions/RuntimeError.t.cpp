/*
 * RuntimeError.t.cpp
 *
 */
#include <tut.h>

#include "System/Exceptions/RuntimeError.hpp"

namespace System
{
namespace Exceptions
{

struct RuntimeErrorTestData
{
};

} // namespace Exceptions
} // namespace System


namespace tut
{
typedef System::Exceptions::RuntimeErrorTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Exceptions/RuntimeError");
}

using namespace std;
using namespace System::Exceptions;

namespace tut
{

namespace
{
struct MyException: public RuntimeError<MyException>
{
  template<typename T>
  MyException(const T &t):
    RuntimeError<MyException>(t)
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
  const MyException    me(someStr);
  const runtime_error &base=me; // this line must compile
  base.what();                  // this suppress warning from compiler
}

} // namespace tut

