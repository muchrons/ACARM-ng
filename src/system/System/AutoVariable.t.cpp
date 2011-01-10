/*
 * AutoVariable.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "System/AutoVariable.hpp"

namespace System
{

struct AutoVariableTestData
{
  struct TestDataHolder
  {
    typedef int TValue;
    TestDataHolder(void):
      _v(-1)
    {
    }
    explicit TestDataHolder(TValue v):
      _v(v)
    {
    }
    TValue  get(void) const
    {
      return _v;
    }
    void deallocate(void)
    {
      if(_v==-1)
        return;

      // ok - throwing strings is ugly, but this is out of the hierarchy
      // class that will not be caught by tut.
      if(_check)
        throw std::string("dealocate(): "
                          "dealocating when not requested");
      _v=-1;
    }

    static void setCheck(bool check)
    {
      _check=check;
    }

    int         _v;     // -1 means: no-ownership
    static bool _check; // if true then class throws on dealocate()
  }; // struct TestDataHolder

  typedef AutoVariable<TestDataHolder> TestAV;

  AutoVariableTestData(void):
    _av1(1),
    _av2(2)
  {
    TestDataHolder::setCheck(false);    // do not check by default
  }

  TestAV mkTestAV(void) const
  {
    TestAV t(123);
    return t;
  }

  TestAV _av1;
  TestAV _av2;
}; // struct AutoVariableTestData

} // namespace System

// static member of TestDataHolder declaration
bool System::AutoVariableTestData::TestDataHolder::_check;


namespace tut
{
typedef System::AutoVariableTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/AutoVariable");
}

using namespace std;
using namespace System;

namespace tut
{

// test if both objects were build are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( _av1.get()==1 );
  ensure( _av2.get()==2 );
}


// check get() and operator()
template<>
template<>
void testObj::test<2>(void)
{
  ensure( _av1.isInitialized() );
  ensure( _av2.isInitialized() );

  TestAV notInit;
  ensure( !notInit.isInitialized() );
}

// isInitialized() test
template<>
template<>
void testObj::test<3>(void)
{
  ensure( _av1.isInitialized() );
  ensure( _av2.isInitialized() );
  _av1.reset(-1);
  _av2.reset(-1);
  ensure( !_av1.isInitialized() );
  ensure( !_av2.isInitialized() );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  TestAV a;
  int tmp=_av1.get();
  a      =_av1;     // ownership passing
  ensure( !_av1.isInitialized() );
  ensure( _av1.get()!=a.get() );
  ensure( a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  TestAV a;
  int    tmp=_av1.get();
  a.reset( _av1.release() );
  ensure( !_av1.isInitialized() );
  ensure( a.get()==tmp );
}

// test copy-constructor
template<>
template<>
void testObj::test<6>(void)
{
  TestAV a(_av1);
  ensure(     a.isInitialized() );
  ensure( !_av1.isInitialized() );
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
    b=_av1;
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
    TestAV b( _av1.release() );
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
    TestAV b(_av1);
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
  a=_av1;
  ensure(     a.isInitialized() );
  ensure( !_av1.isInitialized() );
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
