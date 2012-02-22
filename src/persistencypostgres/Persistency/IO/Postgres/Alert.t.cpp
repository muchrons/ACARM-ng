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
    analyzer_( new Analyzer(42u, "analyzer name", NULL, NULL, NULL ) ),
    analyzers_( analyzer_ ),
    analyzerWithNull_( new Analyzer(44u, "analyzer name", NULL, NULL, NULL )),
    analyzersWithNull_( analyzerWithNull_ ),
    detected_(123444),
    created_(123555),
    severity_(SeverityLevel::INFO),
    certainty_(0.42),
    description_("alert's description"),
    sourceHosts_( generateHosts(2) ),
    targetHosts_( generateHosts(5) ),
    idCache_(new IDCache),
    dbh_(DBHandlePtrNN(new DBHandle(TestConnection::makeParams(), idCache_) ) ),
    conn_(makeConnection() ),
    t_( conn_->createNewTransaction("save_alert_tests") )
  {
    tdba_.fillWithContent1();   // severities are required.
    tdba_.removeAllData();      // remove all other entries
  }

  Persistency::Alert::Hosts generateHosts(unsigned int size) const
  {
    Persistency::Alert::Hosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  TestDBAccess                    tdba_;
  DataCleaner                     dc_;

  const Persistency::Alert::Name  name_;
  const AnalyzerPtrNN             analyzer_;
  Persistency::Alert::Analyzers   analyzers_;
  const AnalyzerPtrNN             analyzerWithNull_;
  Persistency::Alert::Analyzers   analyzersWithNull_;
  const Timestamp                 detected_;
  const Timestamp                 created_;
  const Severity                  severity_;
  const Certainty                 certainty_;
  const std::string               description_;
  const Persistency::Alert::Hosts sourceHosts_;
  const Persistency::Alert::Hosts targetHosts_;

  IDCachePtrNN                    idCache_;
  DBHandlePtrNN                   dbh_;
  IO::ConnectionPtrNN             conn_;
  Transaction                     t_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Alert");
} // unnamned namespace

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
  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzersWithNull_, &detected_, created_, severity_,
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
  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzersWithNull_, NULL, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  t_.commit();
}

} // namespace tut
