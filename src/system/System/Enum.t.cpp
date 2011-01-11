/*
 * Enum.t.cpp
 *
 * test set for Enum.
 *
 */
#include <tut.h>

#include "System/Enum.hpp"

using namespace System;

namespace
{

struct TestClass
{
  struct MyEnum
  {
    typedef enum
    {
      A=1,
      B=10,
      C=100,
      D=1000,
      E=10000
    } Type;
  };

  typedef System::Enum<MyEnum> TestEnum;

  TestClass(void):
    te_( TestEnum::A )
  {
  }

  TestEnum te_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Enum");
}



namespace tut
{

// Class<->Enum comparison
template<>
template<>
void testObj::test<1>(void)
{
  te_=TestEnum::B;
  ensure( "comparing (==) System::Enum with enum value failed", te_==TestEnum::B );
  ensure( "comparing (!=) System::Enum with enum value failed", te_!=TestEnum::A );
}


// Class<->Class comaprison
template<>
template<>
void testObj::test<2>(void)
{
  TestEnum other=TestEnum::C;
  te_=TestEnum::C;
  ensure( "comparison between two System::Enum failed", te_==other );
}


// Assignment tests
template<>
template<>
void testObj::test<3>(void)
{
  TestEnum other1=te_;
  TestEnum other2=TestEnum::B;

  other1=TestEnum::A;
  other2=other1;
  ensure( "assignment to System::Enum from System::Enum failed", other1==other2 );
}

// conversion to long
template<>
template<>
void testObj::test<4>(void)
{
  int x;

  x=te_.toInt();
  ensure(x==1);

  te_=TestEnum::C;
  x=te_.toInt();
  ensure(x==100);
}

// comparison to self
template<>
template<>
void testObj::test<5>(void)
{
  ensure(   te_==te_  );
  ensure( !(te_!=te_) );
}

// conversion to int (old, depracticated call)
template<>
template<>
void testObj::test<6>(void)
{
  te_=TestEnum::C;
  int x=te_.toInt();
  ensure(x==100);
}

} // namespace tut
