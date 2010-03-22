/*
 * Connection.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

namespace
{

struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_( DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(),
                                       idCache_) ) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("save_alert_tests") )
  {
    tdba_.removeAllData();
    tdba_.fillWithContent1();
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

  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  TestDBAccess        tdba_;
  TestConnection      tc_;
  Transaction         t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Connection");
} // unnamed namespace


namespace tut
{

// test doing cleanup entries that would be too old
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("some entries have been deleted",
                conn_->removeEntriesOlderThan(9999, t_), 0);
}

// remove all entries (except those being used ath the moment)
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid number of entries removed",
                conn_->removeEntriesOlderThan(0, t_), 2);
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

