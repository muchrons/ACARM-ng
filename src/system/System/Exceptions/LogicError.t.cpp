/*
 * LogicError.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "System/Exceptions/LogicError.hpp"

namespace System
{
namespace Exceptions
{

struct LogicErrorTestData
{
};

} // namespace Exceptions
} // namespace System


namespace tut
{
typedef System::Exceptions::LogicErrorTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Exceptions/LogicError");
}

using namespace std;
using namespace System::Exceptions;

namespace tut
{

namespace
{
struct MyException: public LogicError<MyException>
{
  template<typename T>
  MyException(const T &t):
    LogicError<MyException>(t)
  {
  }
  template<typename T>
  MyException(const Location &where, const T &t):
    LogicError<MyException>(where, t)
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
  const logic_error &base=me;   // this line must compile
  base.what();                  // this suppress warning from compiler
}

// test c-tor with location info.
template<>
template<>
void testObj::test<3>(void)
{
  const MyException  me(SYSTEM_SAVE_LOCATION, someStr);
  const logic_error &base=me;   // this line must compile
  ensure("no location info", strstr(base.what(), "LogicError.t.cpp:")!=NULL);
}

} // namespace tut

