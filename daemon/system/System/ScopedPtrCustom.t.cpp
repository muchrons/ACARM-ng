/*
 * ScopedPtrCustom.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/mpl/equal.hpp>

#include "System/ScopedPtrCustom.hpp"

using namespace System;

namespace
{
size_t g_deallocationsCount;

void countDeallocs(int *p)
{
  tut::ensure("NULL pointer passed to deallocator", p!=NULL);
  ++g_deallocationsCount;
}

struct TestClass
{
  typedef ScopedPtrCustom<int, countDeallocs> Ptr;

  TestClass(void):
    // NOTE: we can pass any pointer here, since 'deallocator' does not deallocate it
    ptrA_(&tab_[0]),
    ptrB_(&tab_[1])
  {
    tab_[0]=69;
    tab_[1]=42;
    g_deallocationsCount=0;
  }

  int tab_[2];
  Ptr ptrA_;
  Ptr ptrB_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;
factory tf("System/ScopedPtrCustom");
}

namespace tut
{

// test less-than
template<>
template<>
void testObj::test<1>(void)
{
  ensure("less-than failed", ptrA_<ptrB_);
}

// test greater-than
template<>
template<>
void testObj::test<2>(void)
{
  ensure("greater-than failed", ptrB_>ptrA_);
}

// test equality
template<>
template<>
void testObj::test<3>(void)
{
  ensure("equality failed", ptrA_==ptrA_);
}

// test inequality
template<>
template<>
void testObj::test<4>(void)
{
  ensure("inequality failed", ptrA_!=ptrB_);
}

// test default c-tor
template<>
template<>
void testObj::test<5>(void)
{
  Ptr p;
  ensure("default c-tor set not-NULL", p.get()==NULL );
}

// test non-default c-tor
template<>
template<>
void testObj::test<6>(void)
{
  ensure("invalid pointer saved", ptrA_.get()==&tab_[0] );
}

// test d-tor for NULL
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("counter not reset", g_deallocationsCount, 0u);
  {
    Ptr a;
  }
  ensure_equals("deallocator has been called", g_deallocationsCount, 0u);
}

// test d-tor for non-NULL
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals("counter not reset", g_deallocationsCount, 0u);
  {
    Ptr a(&tab_[0]);
  }
  ensure_equals("deallocator not called", g_deallocationsCount, 1u);
}

namespace
{
void doNothing(std::string *v)
{
  ensure("NULL pointer passed to deallocator", v!=NULL);
} // doNothing
} // unnamed namespace

// test arrow operator
template<>
template<>
void testObj::test<9>(void)
{
  std::string                             str("abc");
  ScopedPtrCustom<std::string, doNothing> p(&str);
  ensure_equals("invalid stirng", p->c_str(), str);
}

// test const arrow operator
template<>
template<>
void testObj::test<10>(void)
{
  std::string                                   str("abc");
  const ScopedPtrCustom<std::string, doNothing> p(&str);
  ensure_equals("invalid stirng", p->c_str(), str);
}

// check element's type
template<>
template<>
void testObj::test<11>(void)
{
  ensure("invalid element's type declaration",
         boost::mpl::equal<int, Ptr::element_type>::type::value);
}

// check value's type
template<>
template<>
void testObj::test<12>(void)
{
  ensure("invalid element's value type declaration",
         boost::mpl::equal<int, Ptr::value_type>::type::value);
}

// check element's pionter type
template<>
template<>
void testObj::test<13>(void)
{
  ensure("invalid element's pointer type declaration",
         boost::mpl::equal<int*, Ptr::pointer>::type::value);
}

// check element's reference type
template<>
template<>
void testObj::test<14>(void)
{
  ensure("invalid element's reference type declaration",
         boost::mpl::equal<int&, Ptr::reference>::type::value);
}

// test dereference operator
template<>
template<>
void testObj::test<15>(void)
{
  ensure_equals("invalid value", *ptrB_, 42);
}

// test const derefenrece operator
template<>
template<>
void testObj::test<16>(void)
{
  const Ptr p(&tab_[0]);
  ensure_equals("invalid value", *p, 69);
}

// test get()
template<>
template<>
void testObj::test<17>(void)
{
  ensure("invalid pointer", ptrA_.get()==&tab_[0] );
}

// test const get()
template<>
template<>
void testObj::test<18>(void)
{
  const Ptr &p=ptrA_;
  ensure("invalid pointer", p.get()==&tab_[0] );
}

// test swapping
template<>
template<>
void testObj::test<19>(void)
{
  ptrA_.swap(ptrB_);
  ensure("pointer A has invalid value", ptrA_.get()==&tab_[1] );
  ensure("pointer B has invalid value", ptrB_.get()==&tab_[0] );
  ensure_equals("deallocator has been called", g_deallocationsCount, 0u);
}

// test release
template<>
template<>
void testObj::test<20>(void)
{
  ensure_equals("pre-failed", g_deallocationsCount, 0u);
  ensure("invalid pointer returned", ptrA_.release()==&tab_[0] );
  ensure_equals("deallocator has been called", g_deallocationsCount, 0u);
}

// test reset w-out paramters
template<>
template<>
void testObj::test<21>(void)
{
  ensure_equals("pre-failed", g_deallocationsCount, 0u);
  ptrA_.reset();
  ensure_equals("deallocator not called", g_deallocationsCount, 1u);
  ensure("invalid poitner saved", ptrA_.get()==NULL );
}

// test reset
template<>
template<>
void testObj::test<22>(void)
{
  ensure_equals("pre-failed", g_deallocationsCount, 0u);
  ptrA_.reset(&tab_[1]);
  ensure_equals("deallocator not called", g_deallocationsCount, 1u);
  ensure("invalid poitner saved", ptrA_.get()==&tab_[1] );
}

} // namespace tut
