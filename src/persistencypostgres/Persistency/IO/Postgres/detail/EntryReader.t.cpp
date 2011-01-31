/*
 * EntryReader.t.cpp
 *
 */

#include <tut.h>
#include <boost/algorithm/string.hpp>

#include "Commons/ViaUnorderedCollection.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"
#include "Persistency/IO/Postgres/ExceptionNoEntries.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"

#include "TestHelpers/checkEquality.hpp"
using namespace std;
using namespace pqxx;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;

using boost::algorithm::trim;
using Persistency::IO::Transaction;

namespace
{
struct TestClass
{
  TestClass(void):
    name_("some name"),
    analyzer_( new Analyzer(112233u, "analyzer name", NULL, NULL, NULL) ),
    analyzers_(analyzer_),
    detected_(1000),
    created_(1010),
    severity_(SeverityLevel::INFO),
    certanity_(0.42),
    description_("alert's description"),
    sourceHosts_( generateHosts(2) ),
    targetHosts_( generateHosts(5) ),
    idCache_(new IDCache),
    dbh_(DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_)) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("entry_saver_tests") ),
    es_(t_, *dbh_),
    er_(t_, *dbh_)
  {
  }

  Persistency::Alert::Hosts generateHosts(unsigned int size) const
  {
    Persistency::Alert::Hosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  Persistency::Alert::Hosts generateNULLHosts(unsigned int size) const
  {
    Persistency::Alert::Hosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHostWithNullRefUrl() );
    return out;
  }

  void execSQL(const char *sql)
  {
    dc_.execSQL(sql);
  }

  DataCleaner                     dc_;
  TestDBAccess                    tdba_;

  const Persistency::Alert::Name  name_;
  const AnalyzerPtrNN             analyzer_;
  Persistency::Alert::Analyzers   analyzers_;
  const Timestamp                 detected_;
  const Timestamp                 created_;
  const Severity                  severity_;
  const Certainty                 certanity_;
  const std::string               description_;
  const Persistency::Alert::Hosts sourceHosts_;
  const Persistency::Alert::Hosts targetHosts_;

  IDCachePtrNN                    idCache_;
  DBHandlePtrNN                   dbh_;
  IO::ConnectionPtrNN             conn_;
  Transaction                     t_;
  EntrySaver                      es_;
  EntryReader                     er_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntryReader");

} // unnamed namespace
namespace tut
{

// test for saving NULLS
template<>
template<>
void testObj::test<1>(void)
{
  const Analyzer      a(123u, "analyzer2", NULL, NULL, NULL);
  const DataBaseID    anlzID       = es_.saveAnalyzer(a);
  const AnalyzerPtrNN readAnalyzer = er_.getAnalyzer(anlzID) ;

  ensure("version is not null",readAnalyzer->getVersion().get()==NULL);
  ensure("ip is not null", readAnalyzer->getIP()==NULL);
  ensure("os is not null", readAnalyzer->getOperatingSystem().get()==NULL);
  t_.commit();
}

// restore Analyzer
template<>
template<>
void testObj::test<2>(void)
{
  const Analyzer::Version         anlzVersion("v0.1.0");
  const Analyzer::OperatingSystem anlzOS("wiendols");
  const Analyzer                  a(123u, "analyzer2", anlzVersion, anlzOS, NULL);
  const DataBaseID                anlzID = es_.saveAnalyzer(a);
  const AnalyzerPtrNN             readAnalyzer =  er_.getAnalyzer(anlzID);
  string                          version(readAnalyzer->getVersion().get());
  const string                    os(readAnalyzer->getOperatingSystem().get());
  trim(version);
  ensure_equals("wrong ID", readAnalyzer->getID().get(), 123u);
  ensure_equals("wrong version", version, string(anlzVersion.get()) );
  ensure("ip is not null", readAnalyzer.get()->getIP()==NULL);
  ensure_equals("wrong os", os, string( anlzOS.get()) );
  t_.commit();
}

// restore Alert
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::AlertPtrNN alertPtr (
        new Persistency::Alert(name_, analyzers_, &detected_,
                               created_,
                               severity_, certanity_,description_,
                               sourceHosts_, targetHosts_) );
  // save alert
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_ );
  alert.save();
  DataBaseID alertID = dbh_->getIDCache()->get(alertPtr);

  Persistency::AlertPtrNN a( er_.readAlert( alertID ) );
  TestHelpers::checkEquality(alertPtr, a);
  ensure("invalid name", alertPtr->getName() == a->getName() );
  ensure_equals("invalid description", a->getDescription() , description_ );
  ensure_equals("invalid detected time", *a->getDetectionTime(), detected_);
  ensure_equals("invalid create time", a->getCreationTime(), created_);
  ensure_equals("invalid severity", a->getSeverity().getLevel().toInt(),
                                    severity_.getLevel().toInt());
  ensure_equals("invalid caertainty", a->getCertainty().get(), certanity_.get());
  ensure_equals("vectors are different", (a->getSourceHosts()).size(),
                                         (alertPtr->getSourceHosts()).size() );
  t_.commit();
}

// restore Alert with NULL detection time
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::AlertPtrNN alertPtr( new Persistency::Alert(name_, analyzers_, NULL,
                                                           created_,
                                                           severity_, certanity_,description_,
                                                           sourceHosts_, targetHosts_) );
  // save alert
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_ );
  alert.save();
  DataBaseID alertID = dbh_->getIDCache()->get(alertPtr);

  Persistency::AlertPtrNN a( er_.readAlert( alertID ) );
  ensure("invalid name",a->getName() == name_ );
  ensure_equals("invalid description", a->getDescription() , description_ );
  ensure("detected time is not NULL", a->getDetectionTime()==NULL);
  ensure_equals("invalid create time", a->getCreationTime(), created_);
  ensure_equals("invalid severity", a->getSeverity().getLevel().toInt(),
                                    severity_.getLevel().toInt());
  ensure_equals("invalid caertainty", a->getCertainty().get(), certanity_.get());
  t_.commit();
}

// restore Meta Alert
template<>
template<>
void testObj::test<5>(void)
{

  const std::string malertName("meta alert name");
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  Timestamp(123),
                                  456u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  Persistency::MetaAlertPtrNN ma( er_.readMetaAlert(malertID) );
  ensure("error restoring meta alert", *maPtr == *ma);
  ensure_equals("invalid system-ID",  ma->getID().get(), 456u );
  ensure("invalid meta alert name", ma->getName() == maPtr->getName() );
  ensure_equals("invalid craeted time",  ma->getCreateTime(), Timestamp(123) );
  ensure_equals("invalid severity delta", ma->getSeverityDelta(), 0.1);
  ensure_equals("invalid certainty delta", ma->getCertaintyDelta(), 0.2);

  ReferenceURL::Name nameURL(ma->getReferenceURL()->getName().get());
  ReferenceURL::URL  urlURL(ma->getReferenceURL()->getURL().get());

  ensure("url name is NULL", nameURL.get()!=NULL);
  ensure("url is NULL", urlURL.get()!=NULL);
  ensure("reference url is NULL", ma->getReferenceURL()!=NULL);

  ensure("invalid reference url", *refURL==*ma->getReferenceURL() );
  ensure("invalid url name", nameURL == refURL->getName() );
  ensure("invalid url", urlURL == refURL->getURL());

  t_.commit();
}

// restore Meta Alert with NULL reference URL
template<>
template<>
void testObj::test<6>(void)
{

  std::string malertName("meta alert name");
  ReferenceURLPtr  url;
  Persistency::MetaAlertPtrNN maPtr(
        new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                    0.1, 0.2,
                                    url,
                                    Timestamp(1500),
                                    456u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  Persistency::MetaAlertPtrNN ma( er_.readMetaAlert(malertID) );
  ensure_equals("invalid system-ID",  ma->getID().get(), 456u );
  ensure("invalid meta alert name", strcmp( ma->getName().get(), malertName.c_str() ) == 0);
  ensure_equals("invalid craeted time", ma->getCreateTime(), Timestamp(1500) );
  ensure_equals("invalid severity delta", ma->getSeverityDelta(), 0.1);
  ensure_equals("invalid certainty delta", ma->getCertaintyDelta(), 0.2);
  ensure("reference url is not null", ma->getReferenceURL()==NULL);
  t_.commit();
}

// trying to restore MetaAlerts between
template<>
template<>
void testObj::test<7>(void)
{

  const std::string malertName("meta alert name");
  Timestamp t1( timestampFromString("1970-01-15 07:56:07") );
  Timestamp t2( timestampFromString("1999-10-10 17:56:07") );
  Timestamp t3( timestampFromString("2010-04-22 07:56:07") );

  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr1(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t1,
                                  111u ) );
  Persistency::IO::Postgres::MetaAlert malert1(maPtr1, t_, dbh_);
  Persistency::MetaAlertPtrNN maPtr2(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t2,
                                  222u ) );
  Persistency::IO::Postgres::MetaAlert malert2(maPtr2, t_, dbh_);
  Persistency::MetaAlertPtrNN maPtr3(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t3,
                                  333u ) );
  Persistency::IO::Postgres::MetaAlert malert3(maPtr3, t_, dbh_);
  malert1.save();
  malert2.save();
  malert3.save();
  vector<DataBaseID> malertsBetween;
  malertsBetween = er_.readIDsMalertsBetween(t1, t1);
  ensure_equals("invalid size", malertsBetween.size(), 1);

  malertsBetween = er_.readIDsMalertsBetween(t1, t2);
  ensure_equals("invalid size", malertsBetween.size(), 2);

  malertsBetween = er_.readIDsMalertsBetween(t1, t3);
  ensure_equals("invalid size", malertsBetween.size(), 3);

  malertsBetween = er_.readIDsMalertsBetween(t3, t1);
  ensure_equals("invalid size", malertsBetween.size(), 0);
}

// trying to restore MetaAlerts in use
template<>
template<>
void testObj::test<8>(void)
{
  const std::string malertName("meta alert name");
  const Timestamp t1( timestampFromString("1970-01-15 07:56:07") );
  const Timestamp t2( timestampFromString("1999-10-10 17:56:07") );
  const Timestamp t3( timestampFromString("2010-04-22 07:56:07") );

  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr1(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t1,
                                  10u ) );
  Persistency::IO::Postgres::MetaAlert malert1(maPtr1, t_, dbh_);
  Persistency::MetaAlertPtrNN maPtr2(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t2,
                                  11u  ) );
  Persistency::IO::Postgres::MetaAlert malert2(maPtr2, t_, dbh_);
  Persistency::MetaAlertPtrNN maPtr3(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t3,
                                  12u ) );
  Persistency::IO::Postgres::MetaAlert malert3(maPtr3, t_, dbh_);
  malert1.save();
  malert2.save();
  malert3.save();
  vector<DataBaseID> malertsBetween;
  malertsBetween = er_.readIDsMalertsInUse();
  ensure_equals("invalid size", malertsBetween.size(), 0);

  malert1.markAsUsed();
  malertsBetween = er_.readIDsMalertsInUse();
  ensure_equals("invalid size", malertsBetween.size(), 1);

  malert2.markAsUsed();
  malertsBetween = er_.readIDsMalertsInUse();
  ensure_equals("invalid size", malertsBetween.size(), 2);

  malert3.markAsUsed();
  malertsBetween = er_.readIDsMalertsInUse();
  ensure_equals("invalid size", malertsBetween.size(), 3);
}

// trying to read severity delta
template<>
template<>
void testObj::test<9>(void)
{
  const std::string malertName("meta alert name");
  Timestamp t( timestampFromString("1970-01-15 07:56:07") );
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t,
                                  74u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  ensure_equals("invalid severity delta", er_.getSeverityDelta(malertID), 0.1);
}

// trying to read certainty delta
template<>
template<>
void testObj::test<10>(void)
{
  const std::string malertName("meta alert name");
  Timestamp t( timestampFromString("1970-01-15 07:56:07") );
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  t,
                                  99u ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  ensure_equals("invalid cartainty delta", er_.getCertaintyDelta(malertID), 0.2);
}

// trying to read Alert associated with Meta Alert - negative case
template<>
template<>
void testObj::test<11>(void)
{
  const std::string malertName("meta alert name");
  const Timestamp t( timestampFromString("1970-01-15 07:56:07") );
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  try
  {
    er_.getAlertIDAssociatedWithMetaAlert(malertID);
    fail("reading didn't failed for no entries");
  }
  catch(const ExceptionNoEntries &)
  {
    // this is expected
  }
}

// trying to read Alert associated with Meta Alert
template<>
template<>
void testObj::test<12>(void)
{
  const std::string malertName("meta alert name");
  const Timestamp t( timestampFromString("1970-01-15 07:56:07") );
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get(maPtr);

  Persistency::AlertPtrNN alertPtr ( makeNewAlert() );
  // save alert
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_ );
  alert.save();
  const DataBaseID alertID = dbh_->getIDCache()->get(alertPtr);
  malert.associateWithAlert(alertPtr);

  ensure_equals("invalid ID of Alert associated with Meta Alert",
                er_.getAlertIDAssociatedWithMetaAlert(malertID), alertID );
}

// trying to restore Leaf - negative case
template<>
template<>
void testObj::test<13>(void)
{
  const std::string malertName("meta alert name");
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get(maPtr);

  try
  {
    er_.getLeaf(malertID);
    fail("reading didn't failed for no entries");
  }
  catch(const ExceptionNoEntries &)
  {
    // this is expected
  }
}

// trying to restore Leaf
template<>
template<>
void testObj::test<14>(void)
{
  const std::string malertName("meta alert name");
  const Timestamp t( timestampFromString("1970-01-15 07:56:07") );
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get(maPtr);

  Persistency::AlertPtrNN alertPtr ( makeNewAlert() );
  // save alert
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_ );
  alert.save();
  malert.associateWithAlert(alertPtr);
  Persistency::AlertPtrNN a( er_.getLeaf(malertID) );
  ensure("invalid name", alertPtr->getName() == a->getName() );
  ensure_equals("invalid description", a->getDescription() , alertPtr->getDescription() );
  ensure("invalid detected time", alertPtr->getDetectionTime() == NULL);
  ensure_equals("invalid create time", a->getCreationTime(), alertPtr->getCreationTime());
  ensure_equals("invalid severity", a->getSeverity().getLevel().toInt(),
                                    alertPtr->getSeverity().getLevel().toInt());
  ensure_equals("invalid caertainty", a->getCertainty().get(), alertPtr->getCertainty().get());
  ensure_equals("vectors are different", a->getSourceHosts().size(),
                                         alertPtr->getSourceHosts().size() );
}

// trying to read roots in use
template<>
template<>
void testObj::test<15>(void)
{
  const Restorer::NodesVector outVec = makeNewTree7();
  vector<DataBaseID> roots = er_.readRoots();
  ensure_equals("invalid number of roots", roots.size(), 1u);
}

// trying to read roots in use
template<>
template<>
void testObj::test<16>(void)
{
  const Restorer::NodesVector outVec = makeNewTree6();
  vector<DataBaseID> roots = er_.readRoots();
  ensure_equals("invalid number of roots", roots.size(), 2u);
}

// trying to read Meta Alert children
template<>
template<>
void testObj::test<17>(void)
{
  const std::string malertName("meta alert name");
  const Timestamp t2( timestampFromString("1999-10-10 17:56:07") );
  const Timestamp t3( timestampFromString("2010-04-22 07:56:07") );

  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );

  Persistency::MetaAlertPtrNN maPtr1( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert1(maPtr1, t_, dbh_);

  Persistency::MetaAlertPtrNN maPtr2( makeNewMetaAlert(101u) );
  Persistency::IO::Postgres::MetaAlert malert2(maPtr2, t_, dbh_);

  Persistency::MetaAlertPtrNN maPtr3( makeNewMetaAlert(102u) );
  Persistency::IO::Postgres::MetaAlert malert3(maPtr3, t_, dbh_);
  malert1.save();
  malert2.save();
  malert3.save();
  const DataBaseID malertID1 = dbh_->getIDCache()->get(maPtr1);
  const DataBaseID malertID2 = dbh_->getIDCache()->get(maPtr2);
  const DataBaseID malertID3 = dbh_->getIDCache()->get(maPtr3);
  vector<DataBaseID> children;
  children.push_back(malertID2);
  children.push_back(malertID3);
  malert1.addChild(maPtr2);
  malert1.addChild(maPtr3);
  const vector<DataBaseID> maChildren = er_.readMetaAlertChildren(malertID1);
  ensure_equals("invalid number of children", maChildren.size(), children.size());
  ensure("invalid children", Commons::ViaUnorderedCollection::equal(maChildren, children) );
}

// trying to read Meta Alert children (no children)
template<>
template<>
void testObj::test<18>(void)
{
  const std::string malertName("meta alert name");
  ReferenceURLPtr refURL( makeNewReferenceURL().shared_ptr() );
  Persistency::MetaAlertPtrNN maPtr( makeNewMetaAlert(100u) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  const DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  const vector<DataBaseID> maChildren = er_.readMetaAlertChildren(malertID);
  ensure_equals("invalid number of children", maChildren.size(), 0);
}

// restore Alert with null reported hosts
template<>
template<>
void testObj::test<19>(void)
{
  Persistency::AlertPtrNN alertPtr (
        new Persistency::Alert(name_, analyzers_, &detected_,
                               created_,
                               severity_, certanity_,description_,
                               generateNULLHosts(2), generateHosts(3)) );
  // save alert
  Persistency::IO::Postgres::Alert alert(alertPtr, t_, dbh_ );
  alert.save();
  DataBaseID alertID = dbh_->getIDCache()->get(alertPtr);

  Persistency::AlertPtrNN a( er_.readAlert( alertID ) );
  TestHelpers::checkEquality(alertPtr, a);
  t_.commit();
}

// try reading existing entry from config
template<>
template<>
void testObj::test<20>(void)
{
  execSQL("INSERT INTO config VALUES ('owner', 'key', 'val')");
  Persistency::IO::DynamicConfig::ValueNULL v=er_.readConfigParameter("owner", "key");
  ensure("NULL value returned", v.get()!=NULL );
  ensure_equals("invalid value read", v.get()->get(), string("val") );
}

// try reading non-existing entry from config
template<>
template<>
void testObj::test<21>(void)
{
  execSQL("INSERT INTO config VALUES ('owner', 'key', 'val')");
  Persistency::IO::DynamicConfig::ValueNULL v=er_.readConfigParameter("owner", "key2");
  ensure("something has been read for non-exisiting key", v.get()==NULL );
}

// try reading existing entry from read-only config
template<>
template<>
void testObj::test<22>(void)
{
  execSQL("INSERT INTO config_rdonly VALUES ('owner', 'key', 'val')");
  Persistency::IO::DynamicConfig::Value v=er_.readConstConfigParameter("owner", "key");
  ensure_equals("invalid value read", v.get(), string("val") );
}

// try reading non-existing entry from read-only config
template<>
template<>
void testObj::test<23>(void)
{
  try
  {
    er_.readConstConfigParameter("owner", "key");
    fail("readConstConfigParameter() didn't throw on non-exisitng paramter");
  }
  catch(const Persistency::IO::DynamicConfig::ExceptionNoSuchParameter &)
  {
    // this is expected
  }
}

// try reading serice without NULLS
template<>
template<>
void testObj::test<24>(void)
{
  tdba_.fillWithContent1();
  Persistency::AlertPtrNN                    a =er_.readAlert(3u);
  const Persistency::Alert::Hosts           &rh=a->getTargetHosts();
  ensure_equals("invalid hosts count", rh.size(), 2u);
  Persistency::HostPtrNN                     host=( rh[0]->getIP().to_string()=="127.0.0.3" ? rh[0] : rh[1] );
  const Persistency::Host::Services         &rs  =host->getServices();
  ensure_equals("invalid number of services", rs.size(), 1u);
  const Persistency::Service                &srv =*rs[0];
  ensure("reference URL is NULL", srv.getReferenceURL()!=NULL );
  ensure_equals("invalid reference URL name", srv.getReferenceURL()->getName().get(), string("some reference") );
  ensure_equals("invalid reference URL URL", srv.getReferenceURL()->getURL().get(), string("http://abc.def.org") );
  ensure_equals("invalid serice name", srv.getName().get(), string("sendmail") );
  ensure_equals("invalid serice protocol", srv.getProtocol().get(), string("smtp") );
  ensure_equals("invalid serice port", srv.getPort().get(), 25u);
}

// try reading serice with NULLS
template<>
template<>
void testObj::test<25>(void)
{
  tdba_.fillWithContent1();
  Persistency::AlertPtrNN                    a =er_.readAlert(3u);
  const Persistency::Alert::Hosts           &rh=a->getTargetHosts();
  ensure_equals("invalid hosts count", rh.size(), 2u);
  Persistency::HostPtrNN                     host=( rh[0]->getIP().to_string()=="127.0.0.3" ? rh[1] : rh[0] );
  const Persistency::Host::Services         &rs  =host->getServices();
  ensure_equals("invalid number of services", rs.size(), 1u);
  const Persistency::Service                &srv =*rs[0];
  ensure("reference URL is not NULL", srv.getReferenceURL()==NULL );
  ensure_equals("invalid serice name", srv.getName().get(), string("apache") );
  ensure_equals("invalid serice protocol", srv.getProtocol().get(), string("www") );
  ensure_equals("invalid serice port", srv.getPort().get(), 80u);
}

// try reading process 1
template<>
template<>
void testObj::test<26>(void)
{
  tdba_.fillWithContent1();
  Persistency::AlertPtrNN                     a =er_.readAlert(4u);
  const Persistency::Alert::Hosts            &rh=a->getSourceHosts();
  ensure_equals("invalid hosts count", rh.size(), 1u);
  const Persistency::Host::Processes         &rp=rh[0]->getProcesses();
  ensure_equals("invalid number of processes", rp.size(), 2u);
  ensure("unexpected NULL", rp[0]->getPID()!=NULL );
  const Persistency::Process                 &proc=*( *rp[0]->getPID()==42 ? rp[0] : rp[1] );

  ensure("reference URL is NULL", proc.getReferenceURL()!=NULL );
  ensure_equals("invalid reference URL name", proc.getReferenceURL()->getName().get(), string("other reference") );
  ensure_equals("invalid reference URL URL", proc.getReferenceURL()->getURL().get(), string("https://xyz.org") );

  ensure("path is not NULL", proc.getPath().get()==NULL );

  ensure_equals("invalid name", proc.getName().get(), string("doom.exe") );

  ensure("MD5 is not NULL", proc.getMD5()==NULL );

  ensure("PID is NULL", proc.getPID()!=NULL );
  ensure_equals("invalid PID", *proc.getPID(), 42u);

  ensure("UID is NULL", proc.getUID()!=NULL );
  ensure_equals("invalid UID", *proc.getUID(), 666u);

  ensure("username is NULL", proc.getUsername().get()!=NULL );
  ensure_equals("invalid username", proc.getUsername().get(), string("kain") );

  ensure("params is NULL", proc.getParameters()!=NULL );
  ensure_equals("invalid params", proc.getParameters(), string("-h -e -l") );
}

// try reading process 2
template<>
template<>
void testObj::test<27>(void)
{
  tdba_.fillWithContent1();
  Persistency::AlertPtrNN                     a =er_.readAlert(1u);
  const Persistency::Alert::Hosts            &rh=a->getTargetHosts();
  ensure_equals("invalid hosts count", rh.size(), 1u);
  const Persistency::Host::Processes         &rp=rh[0]->getProcesses();
  ensure_equals("invalid number of processes", rp.size(), 1u);
  const Persistency::Process                 &proc=*rp[0];

  ensure("reference URL is not NULL", proc.getReferenceURL()==NULL );

  ensure("path is NULL", proc.getPath().get()!=NULL );
  ensure_equals("invalid path", proc.getPath().get(), string("/some/path") );

  ensure_equals("invalid name", proc.getName().get(), string("binaryname") );

  ensure("MD5 is NULL", proc.getMD5()!=NULL );
  ensure_equals("invalid MD5", proc.getMD5()->get(), string("01234567890123456789012345678912") );

  ensure("PID is not NULL", proc.getPID()==NULL );

  ensure("UID is not NULL", proc.getUID()==NULL );

  ensure("username is not NULL", proc.getUsername().get()==NULL );

  ensure("params is not NULL", proc.getParameters()==NULL );
}


namespace
{
struct IterationMemory: public Persistency::IO::DynamicConfig::IterationCallback
{
  virtual bool process(const Persistency::IO::DynamicConfig::Key   &k,
                       const Persistency::IO::DynamicConfig::Value &v)
  {
    p_[k.get()]=v.get();
    return true;
  }

  typedef map<string, string> Params;
  Params p_;
}; // struct IterationMemory
} // unnamed namespace

// test iterating over empty parameters set
template<>
template<>
void testObj::test<28>(void)
{
  IterationMemory im;
  er_.iterateConfigParameters("owner", im);
  ensure_equals("something has been read", im.p_.size(), 0u);
}

// iterate over 2 parameters
template<>
template<>
void testObj::test<29>(void)
{
  // insert some data
  execSQL("INSERT INTO config VALUES ('owner', 'key1', 'val1')");
  execSQL("INSERT INTO config VALUES ('owner', 'key2', 'val2')");
  // test
  IterationMemory im;
  er_.iterateConfigParameters("owner", im);
  // sanity checks
  ensure_equals("something has been read", im.p_.size(), 2u);
  IterationMemory::Params::const_iterator it=im.p_.begin();
  // test 1st pair
  ensure_equals("invalid key 1",   it->first,  "key1");
  ensure_equals("invalid value 1", it->second, "val1");
  ++it;
  // test 2nd pair
  ensure_equals("invalid key 2",   it->first,  "key2");
  ensure_equals("invalid value 2", it->second, "val2");
}

// check if owner is respected
template<>
template<>
void testObj::test<30>(void)
{
  // insert some data
  execSQL("INSERT INTO config VALUES ('owner1', 'key1', 'val1')");
  execSQL("INSERT INTO config VALUES ('owner1', 'key2', 'val2')");
  execSQL("INSERT INTO config VALUES ('owner2', 'keyx', 'valy')");
  // test
  IterationMemory im;
  er_.iterateConfigParameters("owner2", im);
  ensure_equals("owner is not checked", im.p_.size(), 1u);
}


namespace
{
struct IterationSkip: public Persistency::IO::DynamicConfig::IterationCallback
{
  IterationSkip(void):
    calls_(0)
  {
  }

  virtual bool process(const Persistency::IO::DynamicConfig::Key   &/*k*/,
                       const Persistency::IO::DynamicConfig::Value &/*v*/)
  {
    ++calls_;
    return false;
  }

  size_t calls_;
}; // struct IterationSkip
} // unnamed namespace

// test skipping iteration when false is returned from callback
template<>
template<>
void testObj::test<31>(void)
{
  // insert some data
  execSQL("INSERT INTO config VALUES ('owner', 'key1', 'val1')");
  execSQL("INSERT INTO config VALUES ('owner', 'key2', 'val2')");
  // test
  IterationSkip is;
  er_.iterateConfigParameters("owner", is);
  // check
  ensure_equals("invalid number of calls", is.calls_, 1u);
}

// TODO: test getSystemIDOfMetaAlert()

} // namespace tut
