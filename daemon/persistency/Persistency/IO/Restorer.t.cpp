/*
 * Restorer.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Restorer.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;
using boost::posix_time::from_iso_string;

namespace
{

class IORestorer: public IO::Restorer
{
public:
  IORestorer(Transaction &t):
    IO::Restorer(t),
    restoreAll_(0),
    restoreBetween_(0)
  {
  }

  virtual void restoreAllInUseImpl(Transaction&, NodesVector&)
  {
    ++restoreAll_;
  }

  virtual void restoreBetweenImpl(Transaction&,
                                  NodesVector&,
                                  const Timestamp&,
                                  const Timestamp&)
  {
    ++restoreBetween_;
  }

  int restoreAll_;
  int restoreBetween_;
}; // class IORestorer


struct TestClass: private TestBase
{
  TestClass(void):
    tapi_( new TestTransactionAPI() ),
    t_(tapi_),
    ior_(t_),
    base_(ior_)
  {
  }

  TransactionAPIAutoPtr      tapi_;
  Transaction                t_;
  IORestorer                 ior_;
  IO::Restorer              &base_;
  IO::Restorer::NodesVector  out_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Restorer");
} // unnamed namespace


namespace tut
{

// test calls to restore all
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("pre-1 failed", ior_.restoreAll_,     0);
  ensure_equals("pre-2 failed", ior_.restoreBetween_, 0);
  base_.restoreAllInUse(out_);
  ensure_equals("invalid number of calls to restore all",
                ior_.restoreAll_,     1);
  ensure_equals("invalid number of calls to restore between",
                ior_.restoreBetween_, 0);
}

// test calls to restore in a given timespec
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-1 failed", ior_.restoreAll_,     0);
  ensure_equals("pre-2 failed", ior_.restoreBetween_, 0);
  base_.restoreBetween(out_,
                       from_iso_string("20100101T235959"),
                       from_iso_string("20100201T235959") );
  ensure_equals("invalid number of calls to restore all",
                ior_.restoreAll_,     0);
  ensure_equals("invalid number of calls to restore between",
                ior_.restoreBetween_, 1);
}

// test throw when to<from
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    base_.restoreBetween(out_,
                         from_iso_string("20100201T235959"),
                         from_iso_string("20100101T235959") );
    fail("restoreBetween() didn't throw on invalid time span");
  }
  catch(const Restorer::ExceptionInvalidTimeSpan&)
  {
    // this is expected
  }
}

// test calls to restore in a equal timespec
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("pre-1 failed", ior_.restoreAll_,     0);
  ensure_equals("pre-2 failed", ior_.restoreBetween_, 0);
  const Timestamp ts=from_iso_string("20100101T235959");
  base_.restoreBetween(out_, ts, ts);
  ensure_equals("invalid number of calls to restore all",
                ior_.restoreAll_,     0);
  ensure_equals("invalid number of calls to restore between",
                ior_.restoreBetween_, 1);
}

// test auto-ptr typedef
template<>
template<>
void testObj::test<5>(void)
{
  RestorerAutoPtr ap( new IORestorer(t_) );
}

} // namespace tut
