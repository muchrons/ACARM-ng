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

} // namespace tut
