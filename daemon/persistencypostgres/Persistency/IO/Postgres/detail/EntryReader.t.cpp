/*
 * EntryReader.t.cpp
 *
 */

#include <tut.h>
#include <boost/algorithm/string.hpp>

#include "TestHelpers/checkEquality.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"

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
    analyzer_( new Analyzer("analyzer name", NULL, NULL, NULL) ),
    analyzers_(analyzer_),
    detected_(1000),
    created_(1010),
    severity_(SeverityLevel::INFO),
    certanity_(0.42),
    description_("alert's description"),
    sourceHosts_( generateReportedHosts(2) ),
    targetHosts_( generateReportedHosts(5) ),
    idCache_(new IDCache),
    dbh_(DBHandlerPtrNN(new DBHandler(TestConnection::makeParams(), idCache_)) ),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("entry_saver_tests") ),
    es_(t_, *dbh_),
    er_(t_, *dbh_)
  {
    tdba_.removeAllData();
  }

  Persistency::Alert::ReportedHosts generateReportedHosts(unsigned int size) const
  {
    Persistency::Alert::ReportedHosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  const Persistency::Alert::Name          name_;
  const AnalyzerPtrNN                     analyzer_;
  Persistency::Alert::SourceAnalyzers     analyzers_;
  const Timestamp                         detected_;
  const Timestamp                         created_;
  const Severity                          severity_;
  const Certainty                         certanity_;
  const std::string                       description_;
  const Persistency::Alert::ReportedHosts sourceHosts_;
  const Persistency::Alert::ReportedHosts targetHosts_;

  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
  EntrySaver          es_;
  EntryReader         er_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntryReader");

} // unnamed namespace
namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
  const Analyzer   a("analyzer2", NULL, NULL, NULL);
  const DataBaseID anlzID = es_.saveAnalyzer(a);
  const AnalyzerPtrNN   readAnalyzer =  er_.getAnalyzer(anlzID) ;

  ensure("version is not null",readAnalyzer->getVersion()==NULL);
  ensure("ip is not null",readAnalyzer->getIP()==NULL);
  ensure("os is not null",readAnalyzer->getOS()==NULL);
  t_.commit();
}

template<>
template<>
void testObj::test<2>(void)
{
  const Analyzer::Version anlzVersion("v0.1.0");
  const Analyzer::OS      anlzOS("wiendols");
  const Analyzer          a("analyzer2", &anlzVersion, &anlzOS, NULL);
  const DataBaseID        anlzID = es_.saveAnalyzer(a);
  const AnalyzerPtrNN     readAnalyzer =  er_.getAnalyzer(anlzID);  // TODO: what is Analyzer is NULL?
  string                  version(readAnalyzer->getVersion()->get());
  const string                  os(readAnalyzer->getOS()->get());
  trim(version);
  ensure_equals("wrong version",version, string(anlzVersion.get()) );
  ensure("ip is not null",readAnalyzer.get()->getIP()==NULL);
  ensure_equals("wrong os", os, string( anlzOS.get()) );
  t_.commit();
}

// save Alert
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
  t_.commit();
}

// save Alert with NULL detection time
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::AlertPtr alertPtr (
      new Persistency::Alert(name_, analyzers_, NULL,
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
  ReferenceURLPtr refURL( makeNewReferenceURL() );
  Persistency::MetaAlertPtrNN maPtr(
      new Persistency::MetaAlert( Persistency::MetaAlert::Name(malertName),
                                  0.1, 0.2,
                                  refURL,
                                  Timestamp(123) ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  Persistency::MetaAlertPtrNN ma( er_.readMetaAlert(malertID) );
  ensure("error restoring meta alert", *maPtr == *ma);
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
                                    Timestamp(1500) ) );
  Persistency::IO::Postgres::MetaAlert malert(maPtr, t_, dbh_);
  malert.save();
  DataBaseID malertID = dbh_->getIDCache()->get(maPtr);
  Persistency::MetaAlertPtrNN ma( er_.readMetaAlert(malertID) );
  ensure("invalid meta alert name", strcmp( ma->getName().get(), malertName.c_str() ) == 0);
  ensure_equals("invalid craeted time", ma->getCreateTime(), Timestamp(1500) );
  ensure_equals("invalid severity delta", ma->getSeverityDelta(), 0.1);
  ensure_equals("invalid certainty delta", ma->getCertaintyDelta(), 0.2);
  ensure("reference url is not null", ma->getReferenceURL()==NULL);
  t_.commit();
}
} // namespace tut
