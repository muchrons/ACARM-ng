/*
 * SmartPtrNotNULL.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Commons/SharedPtrNotNULL.hpp"

using namespace std;
using namespace Commons;

namespace
{

struct TestClass
{
  typedef boost::shared_ptr<int>   BoostPtr;
  typedef SharedPtrNotNULL<int>    PtrNN;

  typedef SharedPtrNotNULL<string> StrPtrNN;

  TestClass(void):
    nn_( new int(42) )
  {
  }

  PtrNN returnValue(void)
  {
    return nn_;
  }

  PtrNN nn_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/SharedPtrNotNULL");
} // unnamed namespace


namespace tut
{

// test dereference
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid value assigned", *nn_, 42);
}

// test throw on NULL
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    PtrNN pnn(NULL);
    fail("c-tor didn't throw on NULL");
  }
  catch(const ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test copy-c-tor
template<>
template<>
void testObj::test<3>(void)
{
  PtrNN pnn(nn_);
  ensure("invalid pointer after copy-ctor", pnn.get()==nn_.get() );
}

// test temporary copy
template<>
template<>
void testObj::test<4>(void)
{
  PtrNN pnn( returnValue() );
  ensure("invalid pointer after copy-ctor", pnn.get()==nn_.get() );
}

// test creationg form boost shared pointer
template<>
template<>
void testObj::test<5>(void)
{
  const BoostPtr bp(new int);
  const PtrNN    pnn(bp);
  ensure("invalid pointer after creating form boost::shared_ptr",
         pnn.get()==bp.get() );
}

// test throw on copying from boost::shared_ptr(NULL)
template<>
template<>
void testObj::test<6>(void)
{
  BoostPtr bp;
  ensure("precondition failed", bp.get()==NULL);
  try
  {
    PtrNN pnn(bp);
    fail("c-tor didn't throw on NULL");
  }
  catch(const ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test convertion operator
template<>
template<>
void testObj::test<7>(void)
{
  BoostPtr bp;
  bp=nn_;
  ensure("invalid pointer value", bp.get()==nn_.get() );
}

// test assignmet from other instance
template<>
template<>
void testObj::test<8>(void)
{
  PtrNN tmp(new int);
  tmp=nn_;
  ensure("invalid pointer value", tmp.get()==nn_.get() );
}

// test assignment from boost::shared_ptr<>
template<>
template<>
void testObj::test<9>(void)
{
  BoostPtr bp(new int);
  nn_=bp;
  ensure("invalid pointer value", bp.get()==nn_.get() );
}

// test const-dereference
template<>
template<>
void testObj::test<10>(void)
{
  const PtrNN tmp=nn_;
  ensure_equals("const-dereference failed", *tmp, 42);
}

// test get()
template<>
template<>
void testObj::test<11>(void)
{
  int *tmp=new int;
  PtrNN nn(tmp);
  ensure("invalid pointer value", nn.get()==tmp);
}

// test const-get
template<>
template<>
void testObj::test<12>(void)
{
  int *tmp=new int;
  const PtrNN nn(tmp);
  ensure("invalid pointer value", nn.get()==tmp);
}

// test throw on assignment from NULL
template<>
template<>
void testObj::test<13>(void)
{
  BoostPtr bp;
  try
  {
    nn_=bp;
    fail("assignment of NULL succeeded");
  }
  catch(const ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test swap
template<>
template<>
void testObj::test<14>(void)
{
  PtrNN tmp(new int);
  int *p1=nn_.get();
  int *p2=tmp.get();
  assert(p1!=p2);
  tmp.swap(nn_);
  ensure("invalid tmp pointer's value", tmp.get()==p1);
  ensure("invalid nn_ pointer's value", nn_.get()==p2);
}

// test arrow operator
template<>
template<>
void testObj::test<15>(void)
{
  const string tmp="Alice in Wonderland";
  StrPtrNN     ptr( new string(tmp) );
  ensure_equals("arrow operator failed", ptr->c_str(), tmp);
}

// test const-arrow operator
template<>
template<>
void testObj::test<16>(void)
{
  const string   tmp="Alice in Wonderland";
  const StrPtrNN ptr( new string(tmp) );
  ensure_equals("arrow operator failed", ptr->c_str(), tmp);
}

} // namespace tut
