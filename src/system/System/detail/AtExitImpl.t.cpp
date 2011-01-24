/*
 * AtExitImpl.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "System/detail/AtExitImpl.hpp"

using namespace std;
using namespace System;
using namespace System::detail;

namespace
{
struct TestClass
{
  std::vector<int> out_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/detail/AtExitImpl");
} // unnamed namespace

// unnamed namespace for helper objects
namespace
{

template<int N>
struct TestDeallocator: public AtExitResourceDeallocator
{
  explicit TestDeallocator(vector<int> *out):
    out_(out)
  {
    assert(out_!=NULL);
  }

  virtual void deallocate(void)
  {
    out_->push_back(N);
  }

  vector<int> *out_;
}; // struct TestDeallocator

} // unnamed namespace

namespace tut
{

// test empty run
template<>
template<>
void testObj::test<1>(void)
{
  AtExitImpl impl;
  impl.deallocateAll();
}

// test dealocation of some elements
template<>
template<>
void testObj::test<2>(void)
{
  AtExitImpl   impl;
  // create
  AtExit::TDeallocPtr res1( new TestDeallocator<1>(&out_) );
  AtExit::TDeallocPtr res2( new TestDeallocator<2>(&out_) );
  AtExit::TDeallocPtr res3( new TestDeallocator<3>(&out_) );
  // register
  impl.registerDeallocator(res1);
  impl.registerDeallocator(res2);
  impl.registerDeallocator(res3);
  // dealocate
  impl.deallocateAll();

  // check output
  ensure_equals("invlid size of output container", out_.size(), 3);
  ensure_equals("unknown unregistration no. 1", out_[0], 3);
  ensure_equals("unknown unregistration no. 2", out_[1], 2);
  ensure_equals("unknown unregistration no. 3", out_[2], 1);
}

// test for single object
template<>
template<>
void testObj::test<3>(void)
{
  AtExitImpl   impl;
  AtExit::TDeallocPtr res( new TestDeallocator<12>(&out_) );
  impl.registerDeallocator(res);
  ensure("problem passing ownership to AtExitImpl", res.get()==NULL);
  impl.deallocateAll();
  // check output
  ensure_equals("invlid size of output container", out_.size(), 1);
  ensure_equals("unknown unregistration", out_[0], 12);
}

} // namespace tut
