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

using namespace System;

namespace
{

struct TestClass
{
  TestClass(void):
    adW_( open("/dev/null",    O_WRONLY) ),
    adR_( open("/dev/urandom", O_RDONLY) ),
    size_( sizeof(buf_) )
  {
  }

  AutoDescriptor adW_;
  AutoDescriptor adR_;
  char           buf_[1024];
  const int      size_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/AutoDescriptor");
}


namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( read ( adR_.get(), buf_, size_)==size_ );   // read is ok.
  ensure( write( adW_.get(), buf_, size_)==size_ );   // write is ok.
}

// isInitialized() test
template<>
template<>
void testObj::test<2>(void)
{
  ensure( adR_.isInitialized() );
  ensure( adW_.isInitialized() );
  adR_.reset(-1);
  adW_.reset(-1);
  ensure( !adR_.isInitialized() );
  ensure( !adW_.isInitialized() );
}

// comparison
template<>
template<>
void testObj::test<3>(void)
{
  ensure( adR_.get()!=adW_.get() );

  // compare of 2 the same descriptors
  int tmp=adR_.get();
  AutoDescriptor c( adR_.release() );
  ensure( c.get()==tmp );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  AutoDescriptor a;
  int tmp=adR_.get();
  a      =adR_;     // ownership passing
  ensure( adR_.get()<0 && a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  AutoDescriptor a;
  int tmp=adR_.get();
  a.reset( adR_.release() );
  ensure( !adR_.isInitialized() );
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
  AutoDescriptor a( adR_ );
  ensure(     a.isInitialized() );
  ensure( !adR_.isInitialized() );
}

} // namespace tut

