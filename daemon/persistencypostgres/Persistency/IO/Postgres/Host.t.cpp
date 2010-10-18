/*
 * Host.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"

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
    analyzerWithNull_( new Analyzer(42u, "analyzer name", NULL, NULL, NULL )),
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
    t_( conn_->createNewTransaction("host_test") )
  {
  }

  Persistency::Alert::Hosts generateHosts(unsigned int size) const
  {
    Persistency::Alert::Hosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  HostPtr makeNewHostWithNullName(void)
  {
    const Persistency::Host::Netmask_v4 mask(mask4_bytes);
    return HostPtr( new Persistency::Host( Persistency::Host::IPv4::from_string("1.2.3.4"),
          &mask,
          "linux",
          makeNewReferenceURL(),
          Persistency::Host::Services(),
          Persistency::Host::Processes(),
          NULL ) );
  }

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
  Persistency::Alert::Hosts       sourceHosts_;
  const Persistency::Alert::Hosts targetHosts_;

  IDCachePtrNN                    idCache_;
  DBHandlePtrNN                   dbh_;
  IO::ConnectionPtrNN             conn_;
  Transaction                     t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/Host");
} // unnamed namespace


namespace tut
{

// trying set name for host
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::HostPtrNN hostPtr( makeNewHostWithNullName() );
  sourceHosts_.push_back(hostPtr);
  Persistency::IO::Postgres::Host host(hostPtr, t_, dbh_);

  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzers_, &detected_, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  host.setName("new host name");
}

// trying set name for host with alredy saved name
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::HostPtrNN hostPtr( makeNewHost() );
  sourceHosts_.push_back(hostPtr);
  Persistency::IO::Postgres::Host host(hostPtr, t_, dbh_);

  Persistency::AlertPtr alertPtr_(new Persistency::Alert(name_, analyzers_, &detected_, created_, severity_,
                                                         certainty_, description_, sourceHosts_, targetHosts_));
  Persistency::IO::Postgres::Alert alert(alertPtr_, t_, dbh_);
  alert.save();
  try
  {
    host.setName("new host name");  // there can be 2 exceptions thrown here, depending
                                    // on implementation call order. it does not matter
                                    // which will be thrown in fact.
   fail("saving didn't failed for already saved name");
  }
  catch(const ExceptionHostNameAlreadySaved &)
  {
    // this is expected
  }
  catch(const ExceptionHostNameAlreadySet &)
  {
    // this is expected
  }
}

} // namespace tut
