/*
 * AutoCptr.t.cpp
 *
 */
#include <tut.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <cassert>

#include "System/AutoCptr.hpp"

namespace System
{

struct AutoCptrTestData
{
  typedef AutoCptr<void> ACP;

  AutoCptrTestData(void):
    _ap1( malloc(2*1024) ),
    _ap2( malloc(4*1024) )
  {
    tut::ensure( _ap1.get()!=NULL );
    tut::ensure( _ap2.get()!=NULL );
  }

  ACP mkNew(int size) const
  {
    ACP a( malloc(size*1024) );
    return a;
  }

  ACP _ap1;
  ACP _ap2;
};

} // namespace System


namespace tut
{
typedef System::AutoCptrTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/AutoCptr");
}

using namespace System;

namespace tut
{

// check read/write from small memory pointer.
template<>
template<>
void testObj::test<1>(void)
{
  char *p1=static_cast<char*>( _ap1.get() );
  for(int i=0; i<2; ++i)
  {
    *(p1+i)='x';            // write.
    ensure( *(p1+i)=='x' ); // stupid, but reads. ;)
  }
}

// test r/w for biger memory part pointer.
template<>
template<>
void testObj::test<2>(void)
{
  char *p2=static_cast<char*>( _ap2.get() );
  for(int i=0; i<4*1024; ++i)
  {
    *(p2+i)='x';            // write.
    ensure( *(p2+i)=='x' ); // stupid, but reads. ;)
  }
}

// test for memory leaks by allocating a lot of memory
template<>
template<>
void testObj::test<3>(void)
{
  const size_t oneMB=1024*1024;
  const size_t size =30*oneMB;      // 30MB
  const size_t off  =size-oneMB;    // next to last MB
  assert(size>oneMB);
  for(size_t i=0; i<200; ++i)
  {
    ACP p( malloc(size) );          // allocate and catch.
    ensure( p.get()!=NULL );        // test if it passed.
    // write something to the memory
    char *ptr=static_cast<char*>( p.get() );
    memset( ptr+off, 0, oneMB );
    // here goes release.
  }
}

// test holding char **
template<>
template<>
void testObj::test<4>(void)
{
  char         **ptr=static_cast<char**>( malloc(303) );
  AutoCptr<char*> p(ptr);           // allocate and catch.
  ensure( p.get()!=NULL );          // test if it passed.
}

// test copy construction
template<>
template<>
void testObj::test<5>(void)
{
  ACP a( mkNew(10) );
  ensure( a.get()!=NULL );          // test if it passed.
}

// test assignment
template<>
template<>
void testObj::test<6>(void)
{
  ACP a( mkNew(10) );
  ACP b( mkNew(10) );
  ensure( a.get()!=NULL );
  ensure( b.get()!=NULL );

  a=b;
  ensure( a.get()!=NULL );          // test if it passed.
  ensure( b.get()==NULL );          // test if it passed.
}

// test forwarding for already deallocated object
template<>
template<>
void testObj::test<7>(void)
{
  ACP a( mkNew(10) );
  ACP b( mkNew(10) );
  a=b;
  // generaly it is ok if the following line does not crash
  a=b;
  ensure( a.get()==NULL );          // test if it passed.
  ensure( b.get()==NULL );          // test if it passed.
}

// test assignment when returning from call
template<>
template<>
void testObj::test<8>(void)
{
  ACP a;
  ensure("pre-condition failed", !a.isInitialized() );
  a=mkNew(20);
  ensure( a.isInitialized() );
}

// test for resource deallocation; this is smoke test - if resource leaks
// valgrind will show this.
template<>
template<>
void testObj::test<9>(void)
{
  ACP a=mkNew(11);
  ensure("pre-condition failed", a.isInitialized() );
  a=mkNew(20);
  ensure( a.isInitialized() );
}

} // namespace tut
