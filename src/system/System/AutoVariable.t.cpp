/*
 * AutoVariable.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "System/AutoVariable.hpp"

using namespace std;
using namespace System;

namespace
{

struct TestClass
{
  struct TestDataHolder
  {
    typedef int TValue;
    TestDataHolder(void):
      v_(-1)
    {
    }
    explicit TestDataHolder(TValue v):
      v_(v)
    {
    }
    TValue  get(void) const
    {
      return v_;
    }
    void deallocate(void)
    {
      if(v_==-1)
        return;

      // ok - throwing strings is ugly, but this is out of the hierarchy
      // class that will not be caught by tut.
      if(check_)
        throw std::string("dealocate(): "
                          "dealocating when not requested");
      v_=-1;
    }

    static void setCheck(bool check)
    {
      check_=check;
    }

    int         v_;     // -1 means: no-ownership
    static bool check_; // if true then class throws on dealocate()
  }; // struct TestDataHolder

  typedef AutoVariable<TestDataHolder> TestAV;

  TestClass(void):
    av1_(1),
    av2_(2)
  {
    TestDataHolder::setCheck(false);    // do not check by default
  }

  TestAV mkTestAV(void) const
  {
    TestAV t(123);
    return t;
  }

  TestAV av1_;
  TestAV av2_;
}; // struct TestClass

// static member of TestDataHolder declaration
bool TestClass::TestDataHolder::check_;

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/AutoVariable");
}

namespace tut
{

// test if both objects were build are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( av1_.get()==1 );
  ensure( av2_.get()==2 );
}


// check get() and operator()
template<>
template<>
void testObj::test<2>(void)
{
  ensure( av1_.isInitialized() );
  ensure( av2_.isInitialized() );

  TestAV notInit;
  ensure( !notInit.isInitialized() );
}

// isInitialized() test
template<>
template<>
void testObj::test<3>(void)
{
  ensure( av1_.isInitialized() );
  ensure( av2_.isInitialized() );
  av1_.reset(-1);
  av2_.reset(-1);
  ensure( !av1_.isInitialized() );
  ensure( !av2_.isInitialized() );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  TestAV a;
  int tmp=av1_.get();
  a      =av1_;     // ownership passing
  ensure( !av1_.isInitialized() );
  ensure( av1_.get()!=a.get() );
  ensure( a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  TestAV a;
  int    tmp=av1_.get();
  a.reset( av1_.release() );
  ensure( !av1_.isInitialized() );
  ensure( a.get()==tmp );
}

// test copy-constructor
template<>
template<>
void testObj::test<6>(void)
{
  TestAV a(av1_);
  ensure(     a.isInitialized() );
  ensure( !av1_.isInitialized() );
}

// test default constructor
template<>
template<>
void testObj::test<7>(void)
{
  TestAV t;                 // should compie fine
  ensure( t.get()==-1 );    // no object should be allocated
}

// reset to another value
template<>
template<>
void testObj::test<8>(void)
{
  TestAV t;
  ensure( !t.isInitialized() );

  t.reset(7);
  ensure( t.get()==7 );
  ensure( t.isInitialized() );

  t.reset(-1);
  ensure( !t.isInitialized() );
}

// test if dealocate() is called ONLY when it is needed
// in case of passing ownership via assignment.
template<>
template<>
void testObj::test<9>(void)
{
  try
  {
    TestDataHolder::setCheck(true);
    TestAV b;
    b=av1_;
    TestDataHolder::setCheck(false);
  }
  catch(const string &ex)
  {
    TestDataHolder::setCheck(false);
    fail( ex.c_str() );
  }
}

// test if dealocate() is called ONLY when it is needed
// when passing ownership via copy-construction, via
// temporary object.
template<>
template<>
void testObj::test<10>(void)
{
  try
  {
    TestDataHolder::setCheck(true);
    TestAV b( mkTestAV() );
    TestDataHolder::setCheck(false);
  }
  catch(const string &ex)
  {
    TestDataHolder::setCheck(false);
    fail( ex.c_str() );
  }
}

// test if dealocate() is called ONLY when it is needed
// when passing ownership via release().
template<>
template<>
void testObj::test<11>(void)
{
  try
  {
    TestDataHolder::setCheck(true);
    TestAV b( av1_.release() );
    TestDataHolder::setCheck(false);
  }
  catch(const string &ex)
  {
    TestDataHolder::setCheck(false);
    fail( ex.c_str() );
  }
}

// test if dealocate() is called ONLY when it is needed
// when passing ownership via copy-construction.
template<>
template<>
void testObj::test<12>(void)
{
  try
  {
    TestDataHolder::setCheck(true);
    TestAV b(av1_);
    TestDataHolder::setCheck(false);
  }
  catch(const string &ex)
  {
    TestDataHolder::setCheck(false);
    fail( ex.c_str() );
  }
}

// test assignment operator
template<>
template<>
void testObj::test<13>(void)
{
  TestAV a;
  a=av1_;
  ensure(     a.isInitialized() );
  ensure( !av1_.isInitialized() );
}

// test assignment from call
template<>
template<>
void testObj::test<14>(void)
{
  TestAV a;
  ensure("pre-condition failed", !a.isInitialized() );
  a=mkTestAV();
  ensure( a.isInitialized() );
}

// test for resource deallocation; this is smoke test - if resource leaks
// valgrind will show this.
template<>
template<>
void testObj::test<15>(void)
{
  TestAV a=mkTestAV();
  ensure("pre-condition failed", a.isInitialized() );
  a=mkTestAV();
  ensure( a.isInitialized() );
}

} // namespace tut
