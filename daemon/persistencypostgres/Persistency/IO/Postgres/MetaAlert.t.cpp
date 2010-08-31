/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

//#include "Persistency/TestHelpers.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
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
    dbh_(DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("save_alert_tests") )
  {
    tdba_.removeAllData();
  }


  void checkIfMetaAlertIsUsed(DataBaseID malertID, const size_t size = 1u)
  {
    stringstream ss;
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    tut::ensure_equals("invalid size",r.size(), size);
  }

  const string        name_;
  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlePtrNN       dbh_;
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
                                                                 Timestamp(),
                                                                 666u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid name", ReaderHelper<string>::readAsNotNull(r[0]["name"]), name_);
  ensure_equals("invalid severity delta", ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]), 0.1);
  ensure_equals("invalid certanity delta", ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]), 0.2);
  ensure_equals("invalid system-ID", ReaderHelper<Persistency::MetaAlert::ID::Numeric>::readAsNotNull(r[0]["sys_id"]), 666u);
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
                                                                 Timestamp(),
                                                                 303u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  checkIfMetaAlertIsUsed(malertID);
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
                                                                 Timestamp(),
                                                                 102u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  checkIfMetaAlertIsUsed(malertID);
  string TriggerName("some trigger name");
  malert.markAsTriggered(TriggerName);
  ss << "SELECT * FROM meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid trigger name", ReaderHelper<string>::readAsNotNull(r[0]["trigger_name"]), TriggerName);
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
                                                                 Timestamp(),
                                                                 42u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  malert.markAsUsed();
  const DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  stringstream ss;
  checkIfMetaAlertIsUsed(malertID);
  malert.markAsUnused();
  checkIfMetaAlertIsUsed(malertID, 0);
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
                                                                 Timestamp(),
                                                                 69u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const  DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  double delta = 0.1;
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid severity delta", ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]), 0.1);
  }
  malert.updateSeverityDelta(delta);
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure("invalid severity delta",
           ( ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]) -  (0.1+delta)) < 0.01 );
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
                                                                 Timestamp(),
                                                                 42u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const  DataBaseID malertID = dbh_->getIDCache()->get( maPtr );
  double delta = 0.1;
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid certanity delta", ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]), 0.2);
  }
  malert.updateCertaintyDelta(delta);
  {
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure("invalid certanity delta",
           ( ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]) - (0.2+delta) ) < 0.01 );
  }
  t_.commit();
}

// trying add child to Meta Alert
template<>
template<>
void testObj::test<7>(void)
{
  Persistency::MetaAlertPtrNN maPtrNode( new Persistency::MetaAlert( Persistency::MetaAlert::Name("meta alert node"),
                                                                     0.1, 0.2,
                                                                     makeNewReferenceURL(),
                                                                     Timestamp(),
                                                                     42u ) );
  Persistency::IO::Postgres::MetaAlert malertNode(maPtrNode, t_, dbh_);
  Persistency::MetaAlertPtrNN maPtrChild( new Persistency::MetaAlert( Persistency::MetaAlert::Name("meta alert child"),
                                                                      0.1, 0.2,
                                                                      makeNewReferenceURL(),
                                                                      Timestamp(),
                                                                      43u ) );
  Persistency::IO::Postgres::MetaAlert malertChild(maPtrChild, t_, dbh_);
  malertNode.save();
  malertChild.save();
  malertNode.addChild( maPtrChild );

  const  DataBaseID malertNodeID = dbh_->getIDCache()->get( maPtrNode );
  const  DataBaseID malertChildID = dbh_->getIDCache()->get( maPtrChild );
  stringstream ss;
  ss << "SELECT * FROM meta_alerts_tree WHERE id_node = " << malertNodeID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid node ID", ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_node"]), malertNodeID);
  ensure_equals("invalid child ID", ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_child"]), malertChildID);
  t_.commit();
}

// trying associate MetaAlert with Alert
template<>
template<>
void testObj::test<8>(void)
{
  // making timestamp for all calls prevents timing issues.
  const Timestamp now=Timestamp(123456);
  Persistency::MetaAlertPtrNN maPtrNode( new Persistency::MetaAlert( Persistency::MetaAlert::Name("meta alert node"),
                                                                     0.1, 0.2,
                                                                     makeNewReferenceURL(),
                                                                     now,
                                                                     1410u ) );
  Persistency::IO::Postgres::MetaAlert malertNode(maPtrNode, t_, dbh_);

  Persistency::Alert::SourceAnalyzers sa( AnalyzerPtrNN( new Analyzer(42u, "analyzer name", NULL, NULL, NULL) ) );
  Persistency::AlertPtr alertPtr(new Persistency::Alert("abc",
                                                        sa,
                                                        NULL,
                                                        now,
                                                        Severity(SeverityLevel::INFO),
                                                        Certainty(0.42),
                                                        "some description",
                                                        Persistency::Alert::ReportedHosts(),
                                                        Persistency::Alert::ReportedHosts()) );
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_);
  malertNode.save();
  alert.save();
  malertNode.associateWithAlert( alertPtr );

  const  DataBaseID malertNodeID = dbh_->getIDCache()->get( maPtrNode );
  const  DataBaseID alertID = dbh_->getIDCache()->get( alertPtr );
  stringstream ss;
  ss << "SELECT * FROM alert_to_meta_alert_map WHERE id_alert = " << alertID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  ensure_equals("invalid Alert ID", ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_meta_alert"]), malertNodeID);
  ensure_equals("invalid MetaAlert ID", ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_alert"]), alertID);
  t_.commit();
}
} // namespace tut

