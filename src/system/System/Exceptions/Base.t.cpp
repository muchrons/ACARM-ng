/*
 * Base.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <cstring>
#include <cassert>

#include "System/Exceptions/Base.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Exceptions/Base");
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
  typedef Base<MyException, std::exception> MyBase;

  template<typename T>
  MyException(const T &t):
    MyBase(t)
  {
  }
  template<typename T>
  MyException(const detail::Location &where, const T &t):
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
  const exception   &base=me;   // this line must compile
  base.what();                  // this suppress warning from compiler
}

// test saving location
template<>
template<>
void testObj::test<5>(void)
{
  const MyException  me(SYSTEM_SAVE_LOCATION, someStr);
  const exception   &base=me;   // this line must compile
  ensure("no/wrong location info", strstr( base.what(), "Base.t.cpp:")!=NULL );
}

// test getting type name
template<>
template<>
void testObj::test<6>(void)
{
  const MyException          me(someStr);
  const MyException::MyBase &base=me;       // this line must compile
  ensure("invalid type name",
         strstr(base.getTypeName().c_str(), "MyException")!=NULL );
}

} // namespace tut
