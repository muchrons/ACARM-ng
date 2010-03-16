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
  int myCall(int ncalls, int tmp=42)
  {
    if(ncalls==0)
    {
      bt_=Backtrace();
      return tmp%3;
    }

    int a;
    if(tmp%3)
      a=myCall(ncalls-1, tmp+1);
    else
      a=myCall(ncalls-1, tmp+2);

    if(a%2)
      tmp+=3;
    return a+tmp;
  } // myCall()

  Backtrace bt_;
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

// test if it shows something
template<>
template<>
void testObj::test<1>(void)
{
  myCall(10);
  ensure("backtrace is too small", bt_.size()>10);
}

// test if backtraces relations works
template<>
template<>
void testObj::test<2>(void)
{
  myCall(10);
  const Backtrace bt1=bt_;
  myCall(21);
  const Backtrace bt2=bt_;
  ensure_equals("backtrace counts does not match", bt2.size()-bt1.size(), 11);
}


// check if toString returns proper stack
template<>
template<>
void testObj::test<3>(void)
{
  myCall(10);
  const string &str =bt_.toString();
  const char   *prev=str.c_str();              // to check stack order
  for(Backtrace::const_iterator it=bt_.begin(); it!=bt_.end(); ++it)
  {
    const char *now=strstr(prev, it->c_str() );
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
  myCall(10);
  for(Backtrace::const_iterator it=bt_.begin(); it!=bt_.end(); ++it)
    ++cnt;
  ensure_equals("invalid size or iterators", cnt, bt_.size() );
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
  const size_t size=256+1;
  myCall(size);                 // this should call reallocation
  ensure("resizing probably failed", bt_.size()>size);
}

// test for multiple buffer enlargement
template<>
template<>
void testObj::test<8>(void)
{
  // the magic value for this test is 256: this is default initial size of table
  // with output addresse for backtrace.
  const size_t size=256*10;
  myCall(size);                 // this should call reallocation
  ensure("resizing probably failed", bt_.size()>size);
}

} // namespace tut
