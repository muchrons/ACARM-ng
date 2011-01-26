/*
 * SmartPtrNotNULL.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/type_traits/is_same.hpp>

#include "System/SharedPtrNotNULL.hpp"

using namespace std;
using namespace System;

namespace
{

struct TestClass
{
  typedef boost::shared_ptr<int>       BoostPtr;
  typedef SharedPtrNotNULL<int>        PtrNN;

  typedef boost::shared_ptr<const int> ConstBoostPtr;
  typedef SharedPtrNotNULL<const int>  ConstPtrNN;

  typedef SharedPtrNotNULL<string> StrPtrNN;

  TestClass(void):
    nn_( new int(42) ),
    other_( new int(42) )
  {
  }

  PtrNN returnValue(void)
  {
    return nn_;
  }

  PtrNN nn_;
  PtrNN other_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/SharedPtrNotNULL");
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
  catch(const ExceptionPointerIsNULL &)
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
  ensure("invalid pointer after creating form boost::shared_ptr", pnn.get()==bp.get() );
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
  catch(const ExceptionPointerIsNULL &)
  {
    // this is expected
  }
}

// test conversion to shared_ptr
template<>
template<>
void testObj::test<7>(void)
{
  BoostPtr bp;
  bp=nn_.shared_ptr();
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
  catch(const ExceptionPointerIsNULL &)
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

// compare with other wrapped-pointer
template<>
template<>
void testObj::test<18>(void)
{
  ensure("different pointers match", !(nn_==other_) );
}

// compare the same wrapped pointers
template<>
template<>
void testObj::test<20>(void)
{
  other_=nn_;
  ensure("the same pointer does not match", nn_==other_);
}

// compare with != of wrapped ptr
template<>
template<>
void testObj::test<22>(void)
{
  ensure("different pointers match", nn_!=other_);
}

// check less-than operator
template<>
template<>
void testObj::test<25>(void)
{
  assert(nn_!=other_);
  ensure("less-than operator does not work properly",
         (other_<nn_ && !(nn_<other_)) || (nn_<other_ && !(other_<nn_)) );
}

// check element's type
template<>
template<>
void testObj::test<26>(void)
{
  ensure("invalid element's type declaration",
         boost::is_same<int, PtrNN::element_type>::type::value);
}

// check value's type
template<>
template<>
void testObj::test<27>(void)
{
  ensure("invalid element's value type declaration",
         boost::is_same<int, PtrNN::value_type>::type::value);
}

// check element's pionter type
template<>
template<>
void testObj::test<28>(void)
{
  ensure("invalid element's pointer type declaration",
         boost::is_same<int*, PtrNN::pointer>::type::value);
}

// check element's reference type
template<>
template<>
void testObj::test<29>(void)
{
  ensure("invalid element's reference type declaration",
         boost::is_same<int&, PtrNN::reference>::type::value);
}

// test conversion operator for constness
template<>
template<>
void testObj::test<30>(void)
{
  const PtrNN   nn( new int(42) );
  ConstBoostPtr bp;
  bp=nn.shared_ptr();
  ensure("invalid pointer value", bp.get()==nn.get() );
}

// test conversion from auto_ptr<>.
template<>
template<>
void testObj::test<31>(void)
{
  std::auto_ptr<int>  ap( new int(42) );
  int                *ptr=ap.get();
  const PtrNN         nn(ap);
  ensure("ap not NULLed", ap.get()==NULL );
  ensure("invalid pointer value", nn.get()==ptr );
}

// test assignment from auto_ptr<>.
template<>
template<>
void testObj::test<32>(void)
{
  std::auto_ptr<int>  ap( new int(42) );
  int                *ptr=ap.get();
  nn_=ap;
  ensure("ap not NULLed", ap.get()==NULL );
  ensure("invalid pointer value", nn_.get()==ptr );
}

// test convertion from non-const to const pointer for self - copy c-tor
template<>
template<>
void testObj::test<33>(void)
{
  PtrNN      a(new int(42));
  ConstPtrNN b(a);
}

// test convertion from non-const to const pointer for boost::shared_ptr - copy c-tor
template<>
template<>
void testObj::test<34>(void)
{
  BoostPtr   a(new int(42));
  ConstPtrNN b(a);
}

// test convertion from non-const to const pointer for std::auto_ptr - copy c-tor
template<>
template<>
void testObj::test<35>(void)
{
  std::auto_ptr<int> a(new int(42));
  ConstPtrNN         b(a);
}

// test convertion from non-const to const pointer for self - assignment
template<>
template<>
void testObj::test<36>(void)
{
  PtrNN      a(new int(42));
  ConstPtrNN b(new int(10));
  b=a;
  ensure_equals("invalid element", *b, 42);
}

// test convertion from non-const to const pointer for boost::shared_ptr - assignment
template<>
template<>
void testObj::test<37>(void)
{
  BoostPtr   a(new int(42));
  ConstPtrNN b(new int(10));
  b=a;
  ensure_equals("invalid element", *b, 42);
}

// test convertion from non-const to const pointer for std::auto_ptr - assignment
template<>
template<>
void testObj::test<38>(void)
{
  std::auto_ptr<int> a(new int(42));
  ConstPtrNN         b(new int(10));
  b=a;
  ensure_equals("invalid element", *b, 42);
}

namespace
{
struct BaseTest
{
};
struct DerivedTest: public BaseTest
{
};
} // unnamed namespace

// test assignment from derived class
template<>
template<>
void testObj::test<39>(void)
{
  SharedPtrNotNULL<DerivedTest> d(new DerivedTest);
  SharedPtrNotNULL<BaseTest>    b(new BaseTest);
  b=d;
}

// test copy c-tr from derived class
template<>
template<>
void testObj::test<40>(void)
{
  SharedPtrNotNULL<DerivedTest> d(new DerivedTest);
  SharedPtrNotNULL<BaseTest>    b(d);
}

// test assignment from derived class, with adding const
template<>
template<>
void testObj::test<41>(void)
{
  SharedPtrNotNULL<DerivedTest>    d(new DerivedTest);
  SharedPtrNotNULL<const BaseTest> b(new BaseTest);
  b=d;
}

// test copy c-tr from derived class, with adding const
template<>
template<>
void testObj::test<42>(void)
{
  SharedPtrNotNULL<DerivedTest>    d(new DerivedTest);
  SharedPtrNotNULL<const BaseTest> b(d);
}

// test converting to shared_ptr - const version
template<>
template<>
void testObj::test<43>(void)
{
  const PtrNN   a(new int(111));
  ConstBoostPtr b=a.shared_ptr();
  b.get();  // discard compiler's warning
}

// test assignment to shared_ptr - const version
template<>
template<>
void testObj::test<44>(void)
{
  const PtrNN   a(new int(111));
  ConstBoostPtr b(new int(42));
  b=a.shared_ptr();
  ensure_equals("invalid value", *b, 111);
}

// test copy-ctor from self to self
template<>
template<>
void testObj::test<45>(void)
{
  PtrNN a(new int(111));
  PtrNN b(a);
}

// test compariosn of const and non-const pointers (compile test)
template<>
template<>
void testObj::test<46>(void)
{
  PtrNN a(new int(111));

  ConstPtrNN b(a);
  a< b;
  a==b;
  a!=b;

  b< a;
  b==a;
  b!=a;

  BoostPtr c( a.shared_ptr() );
  a< c;
  a==c;
  a!=c;

  c< a;
  c==a;
  c!=a;

  ConstBoostPtr d( a.shared_ptr() );
  a< d;
  a==d;
  a!=d;

  d< a;
  d==a;
  d!=a;
}

// test compariosn of base-derived pointers (compile test)
template<>
template<>
void testObj::test<47>(void)
{
  SharedPtrNotNULL<DerivedTest> a(new DerivedTest);

  SharedPtrNotNULL<BaseTest> b(a);
  a< b;
  a==b;
  a!=b;

  b< a;
  b==a;
  b!=a;

  boost::shared_ptr<BaseTest> c( a.shared_ptr() );
  a< c;
  a==c;
  a!=c;

  c< a;
  c==a;
  c!=a;

  boost::shared_ptr<BaseTest> d( a.shared_ptr() );
  a< d;
  a==d;
  a!=d;

  d< a;
  d==a;
  d!=a;
}

// test compariosn of base-derived pointers with NULLs (compile test)
template<>
template<>
void testObj::test<48>(void)
{
  SharedPtrNotNULL<DerivedTest> a(new DerivedTest);

  SharedPtrNotNULL<const BaseTest> b(a);
  a< b;
  a==b;
  a!=b;

  b< a;
  b==a;
  b!=a;

  boost::shared_ptr<const BaseTest> c( a.shared_ptr() );
  a< c;
  a==c;
  a!=c;

  c< a;
  c==a;
  c!=a;

  boost::shared_ptr<const BaseTest> d( a.shared_ptr() );
  a< d;
  a==d;
  a!=d;

  d< a;
  d==a;
  d!=a;
}

} // namespace tut
