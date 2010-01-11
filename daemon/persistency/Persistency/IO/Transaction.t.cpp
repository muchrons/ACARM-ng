/*
 * Transaction.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"

using namespace std;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
  TestClass(void):
    commits_(0),
    rollbacks_(0),
    t_( new TestTransactionAPI(&commits_, &rollbacks_) )
  {
  }

  int                        commits_;
  int                        rollbacks_;
  std::auto_ptr<Transaction> t_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Transaction");
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

// check default rollback
template<>
template<>
void testObj::test<2>(void)
{
  t_.reset(NULL);   // force d-tor call
  ensure_equals("commit called in d-tor",       commits_,   0);
  ensure_equals("rollback not called in d-tor", rollbacks_, 1);
}

// check if no double-rollback is not called in d-tor
template<>
template<>
void testObj::test<3>(void)
{
  t_->rollback();
  ensure_equals("commit called in rollback",       commits_,   0);
  ensure_equals("rollback not called in rollback", rollbacks_, 1);

  t_.reset(NULL);   // force d-tor call
  ensure_equals("commit called in d-tor",                  commits_,   0);
  ensure_equals("rollback called after rollback in d-tor", rollbacks_, 1);
}

// check if double rollbacks are secure
template<>
template<>
void testObj::test<4>(void)
{
  t_->rollback();
  t_->rollback();
  ensure_equals("commit called in rollback", commits_,   0);
  ensure_equals("multiple rollbacks called", rollbacks_, 1);
}

// test if double commits do not fail
template<>
template<>
void testObj::test<5>(void)
{
  t_->commit();
  t_->commit();
  ensure_equals("multiple commits called",   commits_,   1);
  ensure_equals("rollback called in commit", rollbacks_, 0);
}

// test d-tor after commit
template<>
template<>
void testObj::test<6>(void)
{
  t_->commit();     // commit
  t_.reset(NULL);   // force d-tor
  ensure_equals("commit called too many times", commits_,   1);
  ensure_equals("rollback called after commit", rollbacks_, 0);
}

// test if transaction is not active after rollback
template<>
template<>
void testObj::test<7>(void)
{
  t_->rollback();
  try
  {
    t_->ensureIsActive();   // should throw
    fail("not exception has been rasied after ensuring ended transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if ensuring of active transaction pass
template<>
template<>
void testObj::test<8>(void)
{
  t_->ensureIsActive(); // should not throw
}

// test if transaction is not active after commit
template<>
template<>
void testObj::test<9>(void)
{
  t_->commit();
  try
  {
    t_->ensureIsActive();   // should throw
    fail("not exception has been rasied after ensuring ended transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

} // namespace tut
