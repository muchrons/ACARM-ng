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
    null_(NULL),
    nullStr_(NULL)
  {
  }

  const int   i1_;
  const int   i2_;
  const int   i3_;
  const int  *null_;
  const char *nullStr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ViaPointer");
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
  ensure("two NULLs differ", ViaPointer::equal(null_, null_) );
}

// check if one pointer can be NULL
template<>
template<>
void testObj::test<3>(void)
{
  ensure("first NULL doesn't differ", !ViaPointer::equal(null_, &i1_) );
}

// check if second pointer can be NULL
template<>
template<>
void testObj::test<4>(void)
{
  ensure("second NULL doesn't differ", !ViaPointer::equal(&i1_, null_) );
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
  const char a[]="a";
  const char b[]="a";
  ensure("pre-condition failed", a!=b);
  ensure("same C-strings not recognized", ViaPointer::equal(a, b) );
}

// test passing two NULL chars*
template<>
template<>
void testObj::test<9>(void)
{
  ensure("NULL C-strings not equals", ViaPointer::equal(nullStr_, nullStr_) );
}

// test passing one NULL chars*
template<>
template<>
void testObj::test<10>(void)
{
  ensure("NULL equals C-string", !ViaPointer::equal("abc", nullStr_) );
}

// test comparing two NULLs
template<>
template<>
void testObj::test<11>(void)
{
  ensure("NULLs are in wrong order", ViaPointer::lessThan(nullStr_, nullStr_) );
}

// test comparing two NULL with non-NULL
template<>
template<>
void testObj::test<12>(void)
{
  ensure("NULL and non-NULL are in wrong order", ViaPointer::lessThan(nullStr_, "abc") );
}

// test comparing two non-NULL with NULL
template<>
template<>
void testObj::test<13>(void)
{
  ensure("non-NULL and NULL are in wrong order", !ViaPointer::lessThan("abc", nullStr_) );
}

// test comparing two NULL with non-NULL
template<>
template<>
void testObj::test<14>(void)
{
  ensure("NULL and non-NULL are in wrong order", ViaPointer::lessThan(null_, &i1_) );
}

// test comparing two non-NULL with NULL
template<>
template<>
void testObj::test<15>(void)
{
  ensure("non-NULL and NULL are in wrong order", !ViaPointer::lessThan(&i1_, null_) );
}

// test comparing two NULLs
template<>
template<>
void testObj::test<16>(void)
{
  ensure("NULLs are in wrong order", ViaPointer::lessThan(null_, null_) );
}

// check C-string specialization for the same strings
template<>
template<>
void testObj::test<17>(void)
{
  const char a[]="a";
  const char b[]="a";
  ensure("pre-condition failed", a!=b);
  ensure("same C-strings not recognized", !ViaPointer::lessThan(a, b) );
}

// check C-string specialization for less-than strings
template<>
template<>
void testObj::test<18>(void)
{
  const char a[]="a";
  const char b[]="bc";
  ensure("same C-strings not recognized", ViaPointer::lessThan(a, b) );
}

// check C-string specialization for not-less-than strings
template<>
template<>
void testObj::test<19>(void)
{
  const char a[]="a";
  const char b[]="bc";
  ensure("different C-strings not recognized", !ViaPointer::lessThan(b, a) );
}

// check equal values via different pointers
template<>
template<>
void testObj::test<20>(void)
{
  ensure("equal values does not match", !ViaPointer::lessThan(&i1_, &i2_) );
}

// check less-than values
template<>
template<>
void testObj::test<21>(void)
{
  ensure("less-than values sorted wrong way", ViaPointer::lessThan(&i1_, &i3_) );
}

// check not-less-than values
template<>
template<>
void testObj::test<22>(void)
{
  ensure("not-less-than values sorted wrong way", !ViaPointer::lessThan(&i3_, &i1_) );
}

} // namespace tut
