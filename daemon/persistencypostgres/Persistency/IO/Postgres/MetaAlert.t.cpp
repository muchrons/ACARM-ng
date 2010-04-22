/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
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
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid name", ReaderHelper<string>::fromSQLResult(r[0]["name"]), name_);
  ensure_equals("invalid severity delta", ReaderHelper<double>::fromSQLResult(r[0]["severity_delta"]), 0.1);
  ensure_equals("invalid certanity delta", ReaderHelper<double>::fromSQLResult(r[0]["certanity_delta"]), 0.2);
  t_.commit();
}

// trying mark MetaAlert as used
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  t_.commit();
}
// trying mark MetaAlert as triggered
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
  }
  string TriggerName("some trigger name");
  malert.markAsTriggered(TriggerName);
  ss.str("");
  ss << "SELECT * FROM meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid trigger name", ReaderHelper<string>::fromSQLResult(r[0]["trigger_name"]), TriggerName);
  t_.commit();
}
// trying mark MetaAlert as unused
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
  }
  malert.markAsUnused();
  ss.str("");
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 0);
  }
  t_.commit();
}

// trying update Severity delta
template<>
template<>
void testObj::test<5>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const  DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  double delta = 0.1;
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid severity delta", ReaderHelper<double>::fromSQLResult(r[0]["severity_delta"]), 0.1);
  }
  malert.updateSeverityDelta(delta);
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid severity delta", ReaderHelper<double>::fromSQLResult(r[0]["severity_delta"]), 0.1+delta);
  }
  t_.commit();
}

// trying update Certanity delta
template<>
template<>
void testObj::test<6>(void)
{
  Persistency::MetaAlertPtrNN maPtr( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name_),
                                                                 0.1, 0.2,
                                                                 makeNewReferenceURL(),
                                                                 Timestamp() ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const  DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  double delta = 0.1;
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid certanity delta", ReaderHelper<double>::fromSQLResult(r[0]["certanity_delta"]), 0.2);
  }
  malert.updateCertaintyDelta(delta);
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid certanity delta",
                  ReaderHelper<double>::fromSQLResult(r[0]["certanity_delta"]),
                  0.2+delta);
  }
  t_.commit();
}

template<>
template<>
void testObj::test<7>(void)
{

}


} // namespace tut

