/*
 * BaseSimple.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <cassert>

#include "System/Exceptions/BaseSimple.hpp"

namespace System
{
namespace Exceptions
{

struct BaseSimpleTestData
{
};

} // namespace Exceptions
} // namespace System


namespace tut
{
typedef System::Exceptions::BaseSimpleTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Exceptions/BaseSimple");
}

using namespace std;
using namespace System;
using namespace System::Exceptions;

namespace tut
{

namespace
{
struct MyException: public BaseSimple<MyException, std::logic_error>
{
  typedef BaseSimple<MyException, std::logic_error> MyBase;
  template<typename T>
  MyException(const T &t):
    MyBase(t)
  {
  }
  template<typename T>
  MyException(const Location &where, const T &t):
    MyBase(where, t)
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
  const logic_error &base=me;   // this line must compile
  base.what();                  // this suppress warning from compiler
}

// test c-tor with location information
template<>
template<>
void testObj::test<6>(void)
{
  const MyException  me(SYSTEM_SAVE_LOCATION, someStr);
  const logic_error &base=me;   // this line must compile
  ensure("no/wrong location info", strstr( base.what(), "BaseSimple.t.cpp:")!=NULL );
}

// test getting type name
template<>
template<>
void testObj::test<7>(void)
{
  const MyException          me(someStr);
  const MyException::MyBase &base=me;       // this line must compile
  ensure("invalid type name",
         strstr(base.getTypeName().c_str(), "MyException")!=NULL );
}

} // namespace tut
