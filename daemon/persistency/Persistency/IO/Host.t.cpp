/*
 * Host.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Host.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

class IOHost: public IO::Host
{
public:
  IOHost(Persistency::HostPtrNN  host,
         Transaction            &t):
    IO::Host(host, t),
    host_(host),
    calls_(0)
  {
  }

  virtual void setNameImpl(Transaction &, const Persistency::Host::Name &/*name*/)
  {
    ++calls_;
    tut::ensure("invalid pointer", &get()==host_.get() );
  }

  Persistency::HostPtr host_;
  int                  calls_;
}; // class Host


struct TestClass: private TestBase
{
  TestClass(void):
    tapi_( new TestTransactionAPI() ),
    t_(tapi_)
  {
  }

  TransactionAPIAutoPtr tapi_;
  Transaction           t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Host");
} // unnamed namespace


namespace tut
{

// test regular c-tor
template<>
template<>
void testObj::test<1>(void)
{
  IOHost h( makeNewHost(), t_);
}

// test call to set name
template<>
template<>
void testObj::test<2>(void)
{
  HostPtrNN host( new Persistency::Host( Persistency::Host::IPv4::from_string("1.2.3.4"),
                                         NULL,
                                         "some/os",
                                         Persistency::ReferenceURLPtr(),
                                         Persistency::Host::ReportedServices(),
                                         Persistency::Host::ReportedProcesses(),
                                         NULL) );

  IOHost h(host, t_);
  h.setName("xyz");
  ensure_equals("implementatin has not been called", h.calls_, 1);
}

// test creating from NULL
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::HostPtr h;
  assert( h.get()==NULL );
  try
  {
    IOHost host(h, t_);
    fail("NULL host has been accepted");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test throw when creating object with non-active transaction.
template<>
template<>
void testObj::test<4>(void)
{
  t_.rollback();
  try
  {
    IOHost host( makeNewHost(), t_);
    fail("non-active transaction has been accepted");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test throw when setting name with non-active transaction.
template<>
template<>
void testObj::test<5>(void)
{
  IOHost host( makeNewHost(), t_);
  t_.rollback();
  try
  {
    host.setName("baszerr.org");
    fail("setting name didn't failed for non-active transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test throw when name is already set
template<>
template<>
void testObj::test<6>(void)
{
  IOHost h( makeNewHost(), t_);
  try
  {
    h.setName("xyz");
    fail("setName() didn't failed for already set entry");
  }
  catch(const Persistency::Exception &)
  {
    // this is expected
  }
}

} // namespace tut
