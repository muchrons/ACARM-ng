/*
 * TransactionAPI.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/TransactionAPI.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/IO/detail/TransactionAPIOperations.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
  TestClass(void):
    commits_(0),
    rollbacks_(0),
    tapi_( new TestTransactionAPI(&commits_, &rollbacks_) ),
    tapiOps_(*tapi_)
  {
    assert( tapi_.get()!=NULL );
  }

  int                              commits_;
  int                              rollbacks_;
  TransactionAPIAutoPtr            tapi_;
  detail::TransactionAPIOperations tapiOps_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/TransactionAPI");
} // unnamed namespace


namespace tut
{

// ensure no commit/rollback has been performed in c-tor
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("commit called in c-tor",   commits_,   0);
  ensure_equals("rollback called in c-tor", rollbacks_, 0);
}

// check default d-tor
template<>
template<>
void testObj::test<2>(void)
{
  tapi_.reset(NULL);   // force d-tor call
  ensure_equals("commit called in d-tor",   commits_,   0);
  ensure_equals("rollback called in d-tor", rollbacks_, 0);
}

// check if double rollbacks are possible
template<>
template<>
void testObj::test<3>(void)
{
  tapiOps_.rollback();
  tapiOps_.rollback();
  ensure_equals("commit called in rollback", commits_,   0);
  ensure_equals("multiple rollbacks called", rollbacks_, 2);
}

// test if double commits do not fail
template<>
template<>
void testObj::test<4>(void)
{
  tapiOps_.commit();
  tapiOps_.commit();
  ensure_equals("multiple commits called",   commits_,   2);
  ensure_equals("rollback called in commit", rollbacks_, 0);
}

// test if commit and rollback are independent
template<>
template<>
void testObj::test<5>(void)
{
  tapiOps_.commit();
  tapiOps_.rollback();
  tapiOps_.commit();
  tapiOps_.rollback();
  ensure_equals("commit called too many times",   commits_,   2);
  ensure_equals("rollback called too many times", rollbacks_, 2);
}

// test getting name.
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("invalid name", tapi_->getName(), "test_transaction");
}

} // namespace tut
