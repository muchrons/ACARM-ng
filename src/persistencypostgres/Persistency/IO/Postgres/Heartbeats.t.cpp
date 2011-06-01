/*
 * Heartbeats.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Heartbeats.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"

using namespace std;
using namespace pqxx;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using Persistency::IO::Transaction;

namespace
{

template<typename T>
inline pqxx::result execSQL(Transaction &t, const T &sql)
{
  return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()


struct TestClass
{
  TestClass(void):
    idCache_(new IDCache),
    dbh_( DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_) ) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("heartbeats_test") ),
    hb_( conn_->heartbeats("Mr.Heartbeater", t_) )
  {
    tut::ensure("Heartbeats is NULL", hb_.get()!=NULL);
  }

  DataCleaner           dataCleaner_;
  IDCachePtrNN          idCache_;
  DBHandlePtrNN         dbh_;
  IO::ConnectionPtrNN   conn_;
  Transaction           t_;
  IO::HeartbeatsAutoPtr hb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Heartbeats");
} // unnamed namespace


namespace tut
{

// test writing some heartbeat
template<>
template<>
void testObj::test<1>(void)
{
  // write heartbeat
  hb_->report("EvilModule", 404u);

  // check if it was written
  const pqxx::result r=execSQL(t_, "SELECT * FROM heartbeats;");
  ensure_equals("invalid number of entries", r.size(), 1u);

  // check exact values
  {
    const string tmp=ReaderHelper<string>::readAsNotNull(r[0]["owner"]);
    ensure_equals("invalid owner", tmp, "Mr.Heartbeater");
  }
  {
    const string tmp=ReaderHelper<string>::readAsNotNull(r[0]["module"]);
    ensure_equals("invalid owner", tmp, "EvilModule");
  }
  {
    const Timestamp now=Timestamp();
    const Timestamp tmp=ReaderHelper<Timestamp>::readAsNotNull(r[0]["timestamp"]);
    ensure("invalid timestamp", tmp.get()<=now.get() );
  }
  {
    const unsigned int tmp=ReaderHelper<unsigned int>::readAsNotNull(r[0]["timeout"]);
    ensure_equals("invalid timeout", tmp, 404u);
  }
}

// test sending new heartbeat, to replace old one
template<>
template<>
void testObj::test<2>(void)
{
  // write two heartbeat
  hb_->report("EvilModule", 404u);
  hb_->report("EvilModule", 303u);

  // check if it was written
  const pqxx::result r=execSQL(t_, "SELECT * FROM heartbeats;");
  ensure_equals("invalid number of entries", r.size(), 1u);

  // check some random field's value
  {
    const unsigned int tmp=ReaderHelper<unsigned int>::readAsNotNull(r[0]["timeout"]);
    ensure_equals("invalid timeout", tmp, 303u);    // last one should be present
  }
}

} // namespace tut
