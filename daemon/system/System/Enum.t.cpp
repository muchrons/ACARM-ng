/*
 * Enum.t.cpp
 *
 * test set for Enum.
 *
 */
#include <tut.h>

#include "System/Enum.hpp"

namespace System
{

struct EnumTestData
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

  EnumTestData(void):
    _te( TestEnum::A )
  {
  }

  TestEnum _te;
};

} // namespace System


namespace tut
{
typedef System::EnumTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Enum");
}



namespace tut
{

// Class<->Enum comparison
template<>
template<>
void testObj::test<1>(void)
{
  _te = TestEnum::B;
  ensure( "comparing (==) System::Enum with enum value failed",
          _te==TestEnum::B );
  ensure( "comparing (!=) System::Enum with enum value failed",
          _te!=TestEnum::A );
}


// Class<->Class comaprison
template<>
template<>
void testObj::test<2>(void)
{
  TestEnum other=TestEnum::C;
  _te=TestEnum::C;
  ensure( "comparison between two System::Enum failed",
          _te==other );
}


// Assignment tests
template<>
template<>
void testObj::test<3>(void)
{
  TestEnum other1=_te;
  TestEnum other2=TestEnum::B;

  other1=TestEnum::A;
  other2=other1;
  ensure( "assignment to System::Enum from System::Enum failed",
          other1==other2 );
}

// convertion to long
template<>
template<>
void testObj::test<4>(void)
{
  int x;

  x=_te.toInt();
  ensure(x==1);

  _te=TestEnum::C;
  x=_te.toInt();
  ensure(x==100);
}

// comparison to self
template<>
template<>
void testObj::test<5>(void)
{
  ensure(   _te==_te  );
  ensure( !(_te!=_te) );
}

// convertion to int (old, depracticated call)
template<>
template<>
void testObj::test<6>(void)
{
  _te=TestEnum::C;
  int x=_te.toInt();
  ensure(x==100);
}

} // namespace tut
