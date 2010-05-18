/*
 * Host.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"

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
    analyzerWithNull_( new Analyzer("analyzer name", NULL, NULL, NULL )),
    analyzersWithNull_( analyzerWithNull_ ),
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
    t_( conn_->createNewTransaction("host_test") )
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
  const AnalyzerPtrNN                     analyzerWithNull_;
  Persistency::Alert::SourceAnalyzers     analyzersWithNull_;
  const Timestamp                         detected_;
  const Timestamp                         created_;
  const Severity                          severity_;
  const Certainty                         certainty_;
  const std::string                       description_;
  Persistency::Alert::ReportedHosts sourceHosts_;
  const Persistency::Alert::ReportedHosts targetHosts_;

  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandlerPtrNN      dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Host");
} // unnamed namespace


namespace tut
{

//
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::HostPtrNN hostPtr( makeNewHost() );
  sourceHosts_.push_back(hostPtr);
  Persistency::IO::Postgres::Host host(hostPtr, t_, dbh_);

  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzers_, &detected_, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  // TODO: uncomment this when code is fixed.
  //host.setName("new host name");
}

} // namespace tut
