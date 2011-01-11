/*
 * RuntimeError.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "System/Exceptions/RuntimeError.hpp"

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Exceptions/RuntimeError");
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
  template<typename T>
  MyException(const Location &where, const T &t):
    RuntimeError<MyException>(where, t)
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
  const runtime_error &base2=me;// this line must compile
  const exception     &base1=me;// this line must compile
  base1.what();                 // this suppress warning from compiler
  base2.what();                 // this suppress warning from compiler

}
// test for base class
template<>
template<>
void testObj::test<3>(void)
{
  const MyException    me(SYSTEM_SAVE_LOCATION, someStr);
  const exception     &base=me;
  ensure("no locaiton info", strstr(base.what(), "RuntimeError.t.cpp:")!=NULL);
}

} // namespace tut

