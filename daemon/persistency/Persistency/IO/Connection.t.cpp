/*
 * Connection.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace std;
using namespace tut;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

struct TestClass
{
  void ensureCalls(unsigned int pos, int count=1)
  {
    for(unsigned int i=0; i<sizeof(c_.called_)/sizeof(c_.called_[0]); ++i)
      if(i==0)
        ensure_equals("transaction has not been made", c_.called_[0], 1);
      else
        if(i!=pos)
        {
          stringstream ss;
          ss<<"invalid call has been made to call number "<<i;
          ensure_equals( ss.str().c_str(), c_.called_[i], 0);
        }
        else
          ensure_equals("call has not been made given number of times",
                        c_.called_[i], count);
  }

  TestIOConnection c_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Connection");
} // unnamed namespace


namespace tut
{

// test creating new transaction
template<>
template<>
void testObj::test<1>(void)
{
  IO::TransactionAutoPtr tapi=c_.createNewTransaction("X");
  ensure("NULL pointer retuirned", tapi.get()!=NULL );
  ensureCalls(0);
}

// test creating alert-io
template<>
template<>
void testObj::test<2>(void)
{
  const TransactionAutoPtr t( c_.createNewTransaction("abc") );
  AlertAutoPtr ptr=c_.alert( makeNewAlert(), *t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(1);
}

// test creating host-io
template<>
template<>
void testObj::test<3>(void)
{
  const TransactionAutoPtr t( c_.createNewTransaction("abc") );
  HostAutoPtr ptr=c_.host( makeNewHost(), *t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(2);
}

// test creating meta-alert-io
template<>
template<>
void testObj::test<4>(void)
{
  const TransactionAutoPtr t( c_.createNewTransaction("abc") );
  AlertPtrNN   alert=makeNewAlert();
  MetaAlertPtr ma( new Persistency::MetaAlert(alert) );
  MetaAlertAutoPtr ptr=c_.metaAlert(ma, *t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(3);
}

} // namespace tut
