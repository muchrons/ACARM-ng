/*
 * DynamicConfig.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "Persistency/IO/DynamicConfig.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass: private Persistency::TestBase
{
  TestClass(void):
    tapi_( new TestTransactionAPI() ),
    t_(tapi_),
    tdc_("dr_evil", t_)
  {
  }

  void ensureCalls(unsigned int pos) const
  {
    for(unsigned int i=0; i<sizeof(tdc_.calls_)/sizeof(tdc_.calls_[0]); ++i)
      if(i==pos)
        tut::ensure_equals("invalid number of calls", tdc_.calls_[i], 1);
      else
        tut::ensure_equals("invalid call made", tdc_.calls_[i], 0);
  }

  TransactionAPIAutoPtr tapi_;
  Transaction           t_;
  IODynamicConfig       tdc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/DynamicConfig");
} // unnamed namespace


namespace tut
{

// test creating with NULL name
template<>
template<>
void testObj::test<1>(void)
{
  IODynamicConfig tdc(NULL, t_);
}

// test writing parameter
template<>
template<>
void testObj::test<2>(void)
{
  tdc_.write("abc", "42");
  ensureCalls(0);
}

// test reading paramter
template<>
template<>
void testObj::test<3>(void)
{
  ensure("NULL value read", tdc_.read("xyz").get()!=NULL );
  ensureCalls(1);
  ensure_equals("non-NULL value read", *tdc_.read("xyz").get(), std::string("alice has a wonderland") );
}

// test marking as used
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid const-parameter read", tdc_.readConst("pinky"), "i'm const");
  ensureCalls(2);
}

// test creating with non-NULL owner
template<>
template<>
void testObj::test<5>(void)
{
  IODynamicConfig tdc("some owner", t_);
}

} // namespace tut
