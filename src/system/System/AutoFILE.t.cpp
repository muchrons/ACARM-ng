/*
 * AutoFILE.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "System/AutoFILE.hpp"

namespace System
{

struct AutoFILETestData
{
  AutoFILETestData(void):
    _adW( fopen("/dev/null",    "w") ),
    _adR( fopen("/dev/urandom", "r") ),
    _size( sizeof(_buf) )
  {
  }

  AutoFILE     _adW;
  AutoFILE     _adR;
  char         _buf[1024];
  const size_t _size;
};

} // namespace System


namespace tut
{
typedef System::AutoFILETestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/AutoFILE");
}

using namespace System;

namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( fread (_buf, _size, 1, _adR.get() )==1 );   // read is ok.
  ensure( fwrite(_buf, _size, 1, _adW.get() )==1 );   // write is ok.
}

// test copy-constructor
template<>
template<>
void testObj::test<2>(void)
{
  AutoFILE a( _adR );
  ensure(     a.isInitialized() );
  ensure( !_adR.isInitialized() );
}

// isInitialized() test
template<>
template<>
void testObj::test<3>(void)
{
  ensure( _adR.isInitialized() );
  ensure( _adW.isInitialized() );
  _adR.reset(NULL);
  _adW.reset(NULL);
  ensure( !_adR.isInitialized() );
  ensure( !_adW.isInitialized() );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  AutoFILE a;
  FILE *tmp=_adR.get();
  a        =_adR;     // ownership passing
  ensure( _adR.get()==NULL && a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  AutoFILE a;
  FILE *tmp=_adR.get();
  a.reset( _adR.release() );
  ensure( _adR.get()==NULL && a.get()==tmp );
}

// check for resource leaking - this MUST work!
template<>
template<>
void testObj::test<6>(void)
{
  const int maxDescs=64*1024;   // assumption... :(
  for(int i=0; i<maxDescs; ++i)
  {
    FILE *tmp=fopen("/dev/null", "w");     // should always work
    if(tmp==NULL)
      fail("error while opening /dev/null - we may have descriptor leak");
    AutoFILE a(tmp);            // passing the ownership
    // 'a' should be released here
  }
}

} // namespace tut

