/*
 * ViaPointer.t.cpp
 *
 */
#include <tut.h>

#include "Base/ViaPointer.hpp"

using namespace Base;

namespace
{

struct TestClass
{
  TestClass(void):
    i1_(4),
    i2_(4),
    i3_(9),
    null_(NULL)
  {
  }

  const int i1_;
  const int i2_;
  const int i3_;
  const int null_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ViaPOinter");
} // unnamed namespace


namespace tut
{

// check if the same pointers match
template<>
template<>
void testObj::test<1>(void)
{
  ensure("the same pointers don't match", ViaPointer::equal(&i1_, &i1_) );
}

// check if 2 NULLs works fine
template<>
template<>
void testObj::test<2>(void)
{
  ensure("two NULLs differ", ViaPointer::equal(&null_, &null_) );
}

// check if one pointer can be NULL
template<>
template<>
void testObj::test<3>(void)
{
  ensure("first NULL doesn't differ", !ViaPointer::equal(&null_, &i1_) );
}

// check if second pointer can be NULL
template<>
template<>
void testObj::test<4>(void)
{
  ensure("second NULL doesn't differ", !ViaPointer::equal(&i1_, &null_) );
}

// check if the same values for 2 different pointers work file
template<>
template<>
void testObj::test<5>(void)
{
  ensure("different pointers and the same value doesn't match",
         ViaPointer::equal(&i1_, &i2_) );
}

// check if different values are different
template<>
template<>
void testObj::test<6>(void)
{
  ensure("different value does match", !ViaPointer::equal(&i1_, &i3_) );
}

// check C-string specialization for different strings
template<>
template<>
void testObj::test<7>(void)
{
  ensure("different C-strings not recognized", !ViaPointer::equal("a", "b") );
}

// check C-string specialization for the same strings
template<>
template<>
void testObj::test<8>(void)
{
  const char *a="a";
  const char *b="b";
  ensure("pre-condition failed", a!=b);
  ensure("same C-strings not recognized", !ViaPointer::equal(a, b) );
}

} // namespace tut
