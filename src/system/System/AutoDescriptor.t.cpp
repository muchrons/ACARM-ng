/*
 * AutoDescriptor.t.cpp
 *
 * test set for AutoDescriptor.
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "System/AutoDescriptor.hpp"

namespace System
{

struct AutoDescriptorTestData
{
  AutoDescriptorTestData(void):
    _adW( open("/dev/null",    O_WRONLY) ),
    _adR( open("/dev/urandom", O_RDONLY) ),
    _size( sizeof(_buf) )
  {
  }

  AutoDescriptor _adW;
  AutoDescriptor _adR;
  char           _buf[1024];
  const int      _size;
};

} // namespace System


namespace tut
{
typedef System::AutoDescriptorTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/AutoDescriptor");
}

using namespace System;

namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( read ( _adR.get(), _buf, _size)==_size );   // read is ok.
  ensure( write( _adW.get(), _buf, _size)==_size );   // write is ok.
}

// isInitialized() test
template<>
template<>
void testObj::test<2>(void)
{
  ensure( _adR.isInitialized() );
  ensure( _adW.isInitialized() );
  _adR.reset(-1);
  _adW.reset(-1);
  ensure( !_adR.isInitialized() );
  ensure( !_adW.isInitialized() );
}

// comparison
template<>
template<>
void testObj::test<3>(void)
{
  ensure( _adR.get()!=_adW.get() );

  // compare of 2 the same descriptors
  int tmp=_adR.get();
  AutoDescriptor c( _adR.release() );
  ensure( c.get()==tmp );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  AutoDescriptor a;
  int tmp=_adR.get();
  a      =_adR;     // ownership passing
  ensure( _adR.get()<0 && a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  AutoDescriptor a;
  int tmp=_adR.get();
  a.reset( _adR.release() );
  ensure( !_adR.isInitialized() );
  ensure( a.get()==tmp );
}

// check for resource leaking - this MUST work!
template<>
template<>
void testObj::test<6>(void)
{
  const int maxDescs=64*1024;   // assumption... :(
  for(int i=0; i<maxDescs; ++i)
  {
    int tmp=open("/dev/null", O_WRONLY);    // should always work
    if(tmp<0)
      fail("error while opening /dev/null - we may have descriptor leak");
    AutoDescriptor a(tmp);      // passing the ownership
    // 'a' should be released here
  }
}

// test copy-constructor
template<>
template<>
void testObj::test<7>(void)
{
  AutoDescriptor a( _adR );
  ensure(     a.isInitialized() );
  ensure( !_adR.isInitialized() );
}

} // namespace tut

