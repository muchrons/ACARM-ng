/*
 * Backtrace.t.cpp
 *
 */
#include <tut.h>
#include <string.h>

#include "System/Backtrace.hpp"

namespace System
{
struct BacktraceTestData
{
};
} // namespace System


namespace tut
{
typedef System::BacktraceTestData TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/Backtrace");
}

using namespace std;
using namespace System;

namespace tut
{

namespace
{
Backtrace myCall(int recurse)
{
  if(recurse>0)
    return myCall(recurse-1);
  return Backtrace();
} // myCall()
} // unnamed namespace

// test if it shows something
template<>
template<>
void testObj::test<1>(void)
{
  const Backtrace &bt=myCall(10);
  ensure("backtrace is too small", bt.size()>10);
}

// test if backtraces relations works
template<>
template<>
void testObj::test<2>(void)
{
  const Backtrace &bt1=myCall(10);
  const Backtrace &bt2=myCall(21);
  ensure_equals("backtrace counts does not match", 11, bt2.size()-bt1.size() );
}


// check if toString returns proper stack
template<>
template<>
void testObj::test<3>(void)
{
  const char      *prev=NULL;           // to check stack order
  const Backtrace &bt  =myCall(10);
  const string    &str =bt.toString();
  for(Backtrace::const_iterator it=bt.begin(); it!=bt.end(); ++it)
  {
    const char *now=strstr( str.c_str(), (*it).c_str() );
    ensure("expected backtrace string not found", now!=NULL);
    // less-then-equal since entries may repeat in case of recursion
    ensure("invalid order of backtrace", prev<=now);
    prev=now;
  }
}

// test iterators
template<>
template<>
void testObj::test<4>(void)
{
  int cnt=0;
  const Backtrace &bt=myCall(10);
  for(Backtrace::const_iterator it=bt.begin(); it!=bt.end(); ++it)
    ++cnt;
  ensure_equals("invalid size or iterators", cnt, bt.size() );
}

// test copy-construction
template<>
template<>
void testObj::test<5>(void)
{
  const Backtrace bt1;
  const Backtrace bt2(bt1);
}

// test assignment
template<>
template<>
void testObj::test<6>(void)
{
  Backtrace bt1;
  Backtrace bt2;
  bt1=bt2;
}

// test buffor enlargement for big traces.
template<>
template<>
void testObj::test<7>(void)
{
  // the magic value for this test is 256: this is default initial size of table
  // with output addresse for backtrace.
  const size_t    size=256+1;
  const Backtrace bt  =myCall(size);    // this should call reallocation
  ensure("resizing probably failed", bt.size()>size);
}

// test for multiple buffer enlargement
template<>
template<>
void testObj::test<8>(void)
{
  // the magic value for this test is 256: this is default initial size of table
  // with output addresse for backtrace.
  const size_t    size=256*10;
  const Backtrace bt  =myCall(size);    // this should call reallocation
  ensure("resizing probably failed", bt.size()>size);
}

} // namespace tut
