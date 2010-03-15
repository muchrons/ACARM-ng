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

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace std;
using boost::posix_time::from_iso_string;

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

  IO::ConnectionPtrNN makeConnection(void) const
  {
    IO::BackendFactory::Options opts;
    opts["host"]  ="localhost";
    opts["port"]  ="5432";
    opts["dbname"]="acarm_ng_test";
    opts["user"]  ="acarm-ng-daemon";
    opts["pass"]  ="test.daemon";
    return IO::ConnectionPtrNN(
      Persistency::IO::BackendFactory::create("postgres", opts) );
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

//TODO trying save example MetaAlert
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 from_iso_string("2001109T231100") ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
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

