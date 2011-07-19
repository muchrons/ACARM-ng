/*
 * EditableCString.t.cpp
 *
 */
#include <tut.h>

#include "System/EditableCString.hpp"

using namespace std;
using namespace System;

namespace
{

struct TestClass
{
  TestClass(void):
    abc_("abc"),
    cecs_("test")
  {
  }

  EditableCString       abc_;
  const EditableCString cecs_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/EditableCString");
} // unnamed namespace


namespace tut
{

// test getting non-const
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid string", abc_.get(), string("abc") );
}

// test getting const
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid string", cecs_.get(), string("test") );
}

// test setting NULL
template<>
template<>
void testObj::test<3>(void)
{
  const EditableCString ecs(NULL);
  ensure("pointer is not NULL", ecs.get()==NULL );
}

// test setting from string
template<>
template<>
void testObj::test<4>(void)
{
  const EditableCString ecs( string("abc") );
  ensure_equals("invalid string", ecs.get(), string("abc") );
}

// test content swapping method
template<>
template<>
void testObj::test<5>(void)
{
  EditableCString str("alice");
  str.swap(abc_);
  ensure_equals("invalid str 1 content", abc_.get(), string("alice") );
  ensure_equals("invalid str 2 content", str.get(),  string("abc") );
}

// test content swapping swap call
template<>
template<>
void testObj::test<6>(void)
{
  EditableCString str("alice");
  swap(str, abc_);
  ensure_equals("invalid str 1 content", abc_.get(), string("alice") );
  ensure_equals("invalid str 2 content", str.get(),  string("abc") );
}

// test setting char with [] operator
template<>
template<>
void testObj::test<7>(void)
{
  abc_[1]='X';
  ensure_equals("invalid content", abc_.get(),  string("aXc") );
}

// test getting char from const array with [] operator
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals("invalid content", abc_[1], 'b');
}

// test setting last char
template<>
template<>
void testObj::test<9>(void)
{
  abc_[2]='X';
  ensure_equals("invalid content", abc_.get(), string("abX") );
}

// test setting first char
template<>
template<>
void testObj::test<10>(void)
{
  abc_[0]='X';
  ensure_equals("invalid content", abc_.get(), string("Xbc") );
}

} // namespace tut
