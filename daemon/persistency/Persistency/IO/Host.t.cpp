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
         const Transaction      &t):
    IO::Host(host, t),
    host_(host),
    calls_(0)
  {
  }

  virtual void setNameImpl(const Persistency::Host::Name &/*name*/)
  {
    ++calls_;
    tut::ensure("invalid pointer", &get()==host_.get() );
  }

  Persistency::HostPtr host_;
  int                  calls_;
}; // class Host


struct TestClass
{
  TestClass(void):
    tapi_( new TestTransactionAPI() ),
    t_(tapi_)
  {
  }

  TransactionAPIAutoPtr  tapi_;
  const Transaction      t_;
};

typedef TestClass TestClass;
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
  IOHost h( makeNewHost(), t_);
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

} // namespace tut
