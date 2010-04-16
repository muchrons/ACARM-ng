/*
 * Alert.t.cpp
 *
 */

#include <tut.h>

#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"

// TODO: tests

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
    analyzer_( new Analyzer("analyzer name", NULL, NULL, NULL ) ),
    analyzers_( analyzer_ ),
    nullanalyzer_( new Analyzer("analyzer name", NULL, NULL, NULL )),
    nullanalyzers_( nullanalyzer_ ),
    detected_(123444),
    created_(123555),
    severity_(SeverityLevel::INFO),
    certainty_(0.42),
    description_("alert's description"),
    sourceHosts_( generateReportedHosts(2) ),
    targetHosts_( generateReportedHosts(5) ),
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
  // TODO: inadequate variables' names (nullanalyzer is never NULL actually)
  const AnalyzerPtrNN                     nullanalyzer_;
  Persistency::Alert::SourceAnalyzers     nullanalyzers_;
  const Timestamp                         detected_;
  const Timestamp                         created_;
  const Severity                          severity_;
  const Certainty                         certainty_;
  const std::string                       description_;
  const Persistency::Alert::ReportedHosts sourceHosts_;
  const Persistency::Alert::ReportedHosts targetHosts_;

  TestDBAccess            tdba_;
  IDCachePtrNN            idCache_;
  DBHandlerPtrNN          dbh_;
  IO::ConnectionPtrNN     conn_;
  Transaction             t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Alert");
} //unnamned namespace

namespace tut
{

// trying save Alert
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzers_, &detected_, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  t_.commit();
}

// trying save Alert with NULL Host in Analyzers
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, nullanalyzers_, &detected_, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  t_.commit();
}

// trying save Alert with NULL detected time
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, nullanalyzers_, NULL, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  t_.commit();
}

} // namespace tut
