/*
 * IterableEnum.t.cpp
 *
 * test set for Enum.
 *
 */
#include <tut.h>

#include "System/IterableEnum.hpp"

using namespace System;

namespace
{

struct TestClass
{
  struct MyEnum
  {
    typedef enum
    {
      A=10,
      B=11,
      C=12,
      D=13,
      E=14
    } Type;
    enum { Min=A };
    enum { Max=E };
  };

  typedef System::IterableEnum<MyEnum> TestEnum;
  typedef TestEnum::BType              TestEnumBase;

  TestClass(void):
    te_( TestEnum::A )
  {
  }

  TestEnum te_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/IterableEnum");
}


namespace tut
{

// Class<->Enum comparison
template<>
template<>
void testObj::test<1>(void)
{
  te_ = TestEnum::B;
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
  ensure( "assignment to System::Enum from System::Enum failed",
          other1==other2 );
}

// conversion to long
template<>
template<>
void testObj::test<4>(void)
{
  int x;

  x=te_.toInt();
  ensure(x==10);

  te_=TestEnum::C;
  x=te_.toInt();
  ensure(x==12);
}

// test creating from base
template<>
template<>
void testObj::test<5>(void)
{
  TestEnumBase base   =te_;
  TestEnum     derived=base;
  ensure( derived.toInt()==base.toInt() );
}

// test assigning from base
template<>
template<>
void testObj::test<6>(void)
{
  TestEnum     derived=te_;
  TestEnumBase base2  =TestEnumBase::D;
  derived=base2;
  ensure( derived.toInt()==base2.toInt() );
}

// test assiging from pure enum
template<>
template<>
void testObj::test<7>(void)
{
  TestEnum derived=te_;
  derived=TestEnum::D;
  ensure( derived.toInt()==TestEnum::D );
}

// test comparing to self
template<>
template<>
void testObj::test<8>(void)
{
  ensure(   te_==te_  );
  ensure( !(te_!=te_) );
}

// test comparing to base
template<>
template<>
void testObj::test<9>(void)
{
  TestEnumBase base=te_;
  ensure(   te_==base  );
  ensure( !(te_!=base) );
}

// test begin iterator
template<>
template<>
void testObj::test<10>(void)
{
  ensure( *TestEnum::begin()==TestEnum::A );
  ensure( *TestEnum::begin()==10          );
}

// test end iterator
template<>
template<>
void testObj::test<11>(void)
{
  ensure( *TestEnum::end()==TestEnum::E+1 );
  ensure( *TestEnum::end()==14+1          );
}

// test getting size of enum-container
template<>
template<>
void testObj::test<12>(void)
{
  ensure( te_.size()==5 );
  ensure( te_.size()==TestEnum::Size );
}

// const_iterator construction (default)
template<>
template<>
void testObj::test<13>(void)
{
  TestEnum::const_iterator it;
  ensure( *it==TestEnum::A );
}

// const_iterator non-default construction
template<>
template<>
void testObj::test<14>(void)
{
  TestEnum::const_iterator it( TestEnum::B );
  ensure( *it==TestEnum::B );
}

// test equality operator
template<>
template<>
void testObj::test<15>(void)
{
  TestEnum::const_iterator it;
  ensure(it==it);
}

// test inequality operator
template<>
template<>
void testObj::test<16>(void)
{
  TestEnum::const_iterator it;
  ensure( !(it!=it) );
}

// pre-incrementation test
template<>
template<>
void testObj::test<17>(void)
{
  TestEnum::const_iterator it_prev;
  TestEnum::const_iterator it =it_prev;
  TestEnum::const_iterator out=++it;
  ensure( it_prev!=it  );
  ensure( it     ==out );
}

// post-incrementation test
template<>
template<>
void testObj::test<18>(void)
{
  TestEnum::const_iterator it_prev;
  TestEnum::const_iterator it =it_prev;
  TestEnum::const_iterator out=it++;
  ensure( it_prev!=it  );
  ensure( it_prev==out );
}

// dereference operator test
template<>
template<>
void testObj::test<19>(void)
{
  TestEnum::const_iterator it;
  ensure(*it==10);
  ++it;
  ensure(*it==11);
  it++;
  ensure(*it==12);
}

// test if range is valid
template<>
template<>
void testObj::test<20>(void)
{
  TestEnum::const_iterator it;
  for(long i=10; i<=14; ++i)
    ++it;   // should not rise assert
  ensure(*it==15);
}

// test iteration throught elements;
template<>
template<>
void testObj::test<21>(void)
{
  long v=static_cast<int>(TestEnum::Min);
  for(TestEnum::const_iterator it=te_.begin(); it!=te_.end(); ++it)
  {
    ensure(*it==v);
    ++v;
  }
}

// unnamed namespace for helper code
namespace
{
  struct MySmallEnum
  {
    typedef enum
    {
      X
    } Type;
    enum { Min=X };
    enum { Max=X };
  };

  typedef System::IterableEnum<MySmallEnum> TestSmallEnum;
} // unnamed namespace

template<>
template<>
void testObj::test<22>(void)
{
  // this code generated warning on some compilers previously.
  TestSmallEnum te(TestSmallEnum::X);
  te.begin();
}

template<>
template<>
void testObj::test<23>(void)
{
  // smoke test - checks backward compatibility of static calls.
  // it is enought if this test compiles.
  ensure("begin() failed", te_.begin()==TestEnum::begin() );
  ensure("end() failed",   te_.end()  ==TestEnum::end() );
  ensure("size() failed",  te_.size() ==TestEnum::size() );
}

template<>
template<>
void testObj::test<24>(void)
{
  // smoke test for compile time Min Max values presence. it is
  // enought for this test to compile.
  const long min=TestEnum::Min;
  const long max=TestEnum::Max;
  ensure("min and max are equal", min!=max);
}

} // namespace tut
