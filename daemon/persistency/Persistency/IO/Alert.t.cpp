/*
 * Alert.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Alert.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

struct TestIOAlert: public Persistency::IO::Alert
{
  TestIOAlert(Persistency::AlertPtr  alert,
              Transaction           &t):
    Persistency::IO::Alert(alert, t),
    alert_(alert),
    called_(0)
  {
  }

  virtual void saveImpl(Transaction&)
  {
    ++called_;
    tut::ensure("invalid pointer", &get()==alert_.get() );
  }

  Persistency::AlertPtr alert_;
  int                   called_;
}; // struct TestIOAlert

struct TestClass: private TestBase
{
  TestClass(void):
    alert_( new Persistency::Alert("abc",
                                   makeNewAnalyzer(),
                                   NULL,
                                   Timestamp(),
                                   Severity(SeverityLevel::INFO),
                                   Certainty(0.42),
                                   "some description",
                                   Persistency::Alert::ReportedHosts(),
                                   Persistency::Alert::ReportedHosts()) ),
    tapi_( new TestTransactionAPI() ),
    t_(tapi_)
  {
  }

  Persistency::AlertPtrNN alert_;
  TransactionAPIAutoPtr   tapi_;
  Transaction             t_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Alert");
} // unnamed namespace


namespace tut
{

// test regular c-tor
template<>
template<>
void testObj::test<1>(void)
{
  TestIOAlert a(alert_, t_);
}

// test call to save implementation
template<>
template<>
void testObj::test<2>(void)
{
  TestIOAlert a(alert_, t_);
  a.save();
  ensure_equals("invalid number of calls", a.called_, 1);
}

// test creating from NULL
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::AlertPtr ap;
  assert( ap.get()==NULL );
  try
  {
    TestIOAlert a(ap, t_);
    fail("NULL alert has been accepted");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test throw when creating from non-active transaction
template<>
template<>
void testObj::test<4>(void)
{
  t_.rollback();
  try
  {
    TestIOAlert a(alert_, t_);
    fail("c-tor didn't throw on non-active transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test throw when saving with non-active transaction
template<>
template<>
void testObj::test<5>(void)
{
  TestIOAlert a(alert_, t_);
  t_.rollback();
  try
  {
    a.save();
    fail("save() didn't throw on non-active transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

} // namespace tut
