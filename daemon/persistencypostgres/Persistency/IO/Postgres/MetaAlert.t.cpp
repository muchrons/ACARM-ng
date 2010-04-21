/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

namespace
{

struct TestClass
{
  TestClass(void):
    name_("some name"),
    idCache_(new IDCache),
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("save_alert_tests") )
  {
    tdba_.removeAllData();
  }

  const string        name_;
  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/MetaAlert");
} // unnamed namespace


namespace tut
{

// trying save example MetaAlert
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  t_.commit();
}

//TODO tests
template<>
template<>
void testObj::test<2>(void)
{

}
//TODO tests
template<>
template<>
void testObj::test<3>(void)
{

}
//TODO tests
template<>
template<>
void testObj::test<4>(void)
{

}

} // namespace tut

