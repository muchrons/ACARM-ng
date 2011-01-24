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

using namespace System;

namespace
{

struct TestClass
{
  TestClass(void):
    adW_( fopen("/dev/null",    "w") ),
    adR_( fopen("/dev/urandom", "r") ),
    size_( sizeof(buf_) )
  {
  }

  AutoFILE     adW_;
  AutoFILE     adR_;
  char         buf_[1024];
  const size_t size_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/AutoFILE");
}


namespace tut
{

// test if both descriptors are file.
template<>
template<>
void testObj::test<1>(void)
{
  ensure( fread (buf_, size_, 1, adR_.get() )==1 );   // read is ok.
  ensure( fwrite(buf_, size_, 1, adW_.get() )==1 );   // write is ok.
}

// test copy-constructor
template<>
template<>
void testObj::test<2>(void)
{
  AutoFILE a(adR_);
  ensure(     a.isInitialized() );
  ensure( !adR_.isInitialized() );
}

// isInitialized() test
template<>
template<>
void testObj::test<3>(void)
{
  ensure( adR_.isInitialized() );
  ensure( adW_.isInitialized() );
  adR_.reset(NULL);
  adW_.reset(NULL);
  ensure( !adR_.isInitialized() );
  ensure( !adW_.isInitialized() );
}

// ownership passing
template<>
template<>
void testObj::test<4>(void)
{
  AutoFILE a;
  FILE *tmp=adR_.get();
  a        =adR_;     // ownership passing
  ensure( adR_.get()==NULL && a.get()==tmp );
}

// reseting and releasing
template<>
template<>
void testObj::test<5>(void)
{
  AutoFILE a;
  FILE *tmp=adR_.get();
  a.reset( adR_.release() );
  ensure( adR_.get()==NULL && a.get()==tmp );
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

