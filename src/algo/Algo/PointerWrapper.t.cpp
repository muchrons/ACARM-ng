/*
 * PointerWrapper.t.cpp
 *
 */
#include <tut.h>
#include <boost/mpl/equal.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Algo/PointerWrapper.hpp"

using boost::is_same;
using namespace Algo;

namespace
{

struct Data
{
  void doSth(void)
  {
  }
  void doSthConst(void) const
  {
  }
}; // struct Data


struct TestClass
{
  TestClass(void):
    rp_(new Data),
    sp_(rp_),
    spnn_(sp_)
  {
  }

  Data                            *rp_;
  boost::shared_ptr<Data>          sp_;
  Commons::SharedPtrNotNULL<Data>  spnn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/PointerWrapper");
} // unnamed namespace


namespace tut
{

// test operation on raw pointer
template<>
template<>
void testObj::test<1>(void)
{
  typedef Data*                   PtrImpl;
  typedef PointerWrapper<PtrImpl> Ptr;
  Ptr p(rp_);
  p.get()->doSth();
  p.get()->doSthConst();
  p->doSth();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<Data>, Ptr::smart_pointer>::type::value);
}

// test const raw pointer
template<>
template<>
void testObj::test<2>(void)
{
  typedef const Data*             PtrImpl;
  typedef PointerWrapper<PtrImpl> Ptr;
  Ptr p(rp_);
  //p.get()->doSth(); // cannot call non-const
  //p->doSth();
  p.get()->doSthConst();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<const Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<const Data>, Ptr::smart_pointer>::type::value);
}

// test shared pointer
template<>
template<>
void testObj::test<3>(void)
{
  typedef boost::shared_ptr<Data> PtrImpl;
  typedef PointerWrapper<PtrImpl> Ptr;
  Ptr p(sp_);
  p.get()->doSth();
  p->doSth();
  p.get()->doSthConst();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<Data>, Ptr::smart_pointer>::type::value);
}

// test const shared pointer
template<>
template<>
void testObj::test<4>(void)
{
  typedef boost::shared_ptr<const Data> PtrImpl;
  typedef PointerWrapper<PtrImpl>       Ptr;
  Ptr p(sp_);
  //p.get()->doSth(); // cannot call non-const
  //p->doSth();
  p.get()->doSthConst();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<const Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<const Data>, Ptr::smart_pointer>::type::value);
}

// test shared pointer not null
template<>
template<>
void testObj::test<5>(void)
{
  typedef Commons::SharedPtrNotNULL<Data> PtrImpl;
  typedef PointerWrapper<PtrImpl>         Ptr;
  Ptr p(spnn_);
  p.get()->doSth();
  p->doSth();
  p.get()->doSthConst();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<Data>, Ptr::smart_pointer>::type::value);
}

// test const shared pointer not null
template<>
template<>
void testObj::test<6>(void)
{
  typedef Commons::SharedPtrNotNULL<const Data> PtrImpl;
  typedef PointerWrapper<PtrImpl>               Ptr;
  Ptr p(spnn_);
  //p.get()->doSth();
  //p->doSth();
  p.get()->doSthConst();
  p->doSthConst();
  ensure("invalid type of pointer", is_same<const Data*, Ptr::pointer>::type::value);
  ensure("invalid type of smart pointer", is_same<Commons::SharedPtrNotNULL<const Data>, Ptr::smart_pointer>::type::value);
}

// test exception when raw pointer is null
template<>
template<>
void testObj::test<7>(void)
{
  Data *null=NULL;
  try
  {
    PointerWrapper<Data*> p(null);
    fail("exception not throw on NULL pointer");
  }
  catch(const Exception&)
  {
    // this is expected
  }
}

// test exception when shared pointer is null
template<>
template<>
void testObj::test<8>(void)
{
  boost::shared_ptr<Data> null;
  try
  {
    PointerWrapper< boost::shared_ptr<Data> > p(null);
    fail("exception not throw on NULL pointer");
  }
  catch(const Exception&)
  {
    // this is expected
  }
}

// test common conversions from Commons::SharedPtrNotNULL<> - smoke test (must compile)
template<>
template<>
void testObj::test<9>(void)
{
  PointerWrapper< Commons::SharedPtrNotNULL<Data> > ptr(spnn_);

  PointerWrapper< Commons::SharedPtrNotNULL<Data> >       ptr1(ptr);
  PointerWrapper< Commons::SharedPtrNotNULL<const Data> > ptr2(ptr);

  PointerWrapper< boost::shared_ptr<Data> >       ptr3(ptr);
  PointerWrapper< boost::shared_ptr<const Data> > ptr4(ptr);

  PointerWrapper<Data*>       ptr5(ptr);
  PointerWrapper<const Data*> ptr6(ptr);
}

// test common conversions from boost:shared_ptr<> - smoke test (must compile)
template<>
template<>
void testObj::test<10>(void)
{
  PointerWrapper< boost::shared_ptr<Data> > ptr(sp_);

  PointerWrapper< boost::shared_ptr<Data> >       ptr1(ptr);
  PointerWrapper< boost::shared_ptr<const Data> > ptr2(ptr);

  PointerWrapper<Data*>       ptr3(ptr);
  PointerWrapper<const Data*> ptr4(ptr);
}

// test common conversions from raw pointers - smoke test (must compile)
template<>
template<>
void testObj::test<11>(void)
{
  PointerWrapper<Data*> ptr(sp_);

  PointerWrapper<Data*>       ptr1(ptr);
  PointerWrapper<const Data*> ptr2(ptr);
}

// test for element_type typedef
template<>
template<>
void testObj::test<12>(void)
{
  typedef boost::shared_ptr<Data> PtrImpl;
  typedef PointerWrapper<PtrImpl> Ptr;
  ensure("invalid typedef for element_type", is_same<PtrImpl, Ptr::element_type>::type::value);
}

// test for value_type typedef
template<>
template<>
void testObj::test<13>(void)
{
  typedef boost::shared_ptr<Data> PtrImpl;
  typedef PointerWrapper<PtrImpl> Ptr;
  ensure("invalid typedef for value_type", is_same<PtrImpl, Ptr::value_type>::type::value);
}

} // namespace tut
