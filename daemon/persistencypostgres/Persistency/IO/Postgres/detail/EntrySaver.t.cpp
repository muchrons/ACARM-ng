/*
 * EntrySaver.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;
using namespace std;
using namespace pqxx;

namespace
{

const Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                   0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                               } };


struct TestClass
{
  TestClass(void):
    name_("some name"),
    analyzer_( new Analyzer("analyzer name", HostPtr() ) ),
    detected_(boost::gregorian::from_simple_string("2001-10-9")),
    created_(boost::gregorian::from_simple_string("2001-10-10")),
    severity_(SeverityLevel::INFO),
    certanity_(0.42),
    description_("alert's description"),
    sourceHosts_( generateReportedHosts(2) ),
    targetHosts_( generateReportedHosts(5) ),
    idCache_(new IDCache),
    dbh_(TestConnection::makeParams(), idCache_),
    conn_( makeConnection() ),
    t_( conn_->createNewTransaction("entry_saver_tests") ),
    es_(t_, dbh_),
    pid_(42),
    uid_(69),
    url_( new ReferenceURL("url1", "http://www.lmgtfy.com") ),
    proc_( "/a/b/c/d",
           "some.proc",
           NULL,
           &pid_,
           &uid_,
           "johndoe",
           "-a -b -c",
           url_ ),
    mask4_( Host::Netmask_v4(mask4_bytes) ),
    mask6_( Host::Netmask_v6(mask6_bytes) )
  {
    //tdba_.removeAllData();
  }

  IO::ConnectionPtrNN makeConnection(void) const
  {
    IO::BackendFactory::Options opts;
    opts["host"]  ="localhost";
    opts["port"]  ="5432";
    opts["dbname"]="acarm_ng_test";
    opts["user"]  ="acarm-ng-daemon";
    opts["pass"]  ="test.daemon";
    return Persistency::IO::BackendFactory::create("postgres", opts);
  }

  Alert::ReportedHosts generateReportedHosts(unsigned int size) const
  {
    Alert::ReportedHosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  const Alert::Name          name_;
  const AnalyzerPtr          analyzer_;
  const Timestamp            detected_;
  const Timestamp            created_;
  const Severity             severity_;
  const Certainty            certanity_;
  const std::string               description_;
  const Alert::ReportedHosts sourceHosts_;
  const Alert::ReportedHosts targetHosts_;


  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandler           dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
  EntrySaver          es_;

  const pid_t       pid_;
  const int         uid_;
  ReferenceURLPtrNN url_;
  const Process     proc_;

  const Host::Netmask_v4 mask4_;
  const Host::Netmask_v6 mask6_;

};

typedef TestClass                  TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntrySaver");
} // unnamed namespace


namespace tut
{

// try saving example Process - smoke test
template<>
template<>
void testObj::test<1>(void)
{
  // TODO: host has to be saved first
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  es_.saveProcess(thostID, proc_);
  t_.commit();
}

// check returned IDs
template<>
template<>
void testObj::test<2>(void)
{
  // TODO: host has to be saved first
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID id1=es_.saveProcess(thostID, proc_);
  const DataBaseID id2=es_.saveProcess(thostID, proc_);
  ensure("invalid ids returned", id1<=id2+1);
}

// test if call throws when invalid ID is passed
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    es_.saveProcess(42, proc_);
    fail("saving didn't failed for non-exising process ID");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

// try saving example Alert
template<>
template<>
void testObj::test<4>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  es_.saveAlert(anlzID,a);
  t_.commit();
}

// try saving example Service
template<>
template<>
void testObj::test<5>(void)
{ 
  const Service ti("mail daemon", 25, "smtp", makeNewReferenceURL() );
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  es_.saveService(thostID,ti);
  t_.commit();
}

// try saving example Analyzer
template<>
template<>
void testObj::test<6>(void)
{
  HostPtr host=makeNewHost();
  const Analyzer a("analyzer2", host);
  DataBaseID hostID = es_.saveHostData(*host);
  es_.saveAnalyzer(&hostID,a);
  t_.commit();
}

// try saving example Analyzer with NULL id_host
template<>
template<>
void testObj::test<7>(void)
{
  const string anlzName("analyzer3");
  const Analyzer a(anlzName,makeNewHost());
  DataBaseID anlzID = es_.saveAnalyzer(NULL,a);

  stringstream ss;
  string name;
  ss << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);
  
  r[0]["name"].to(name);
  ensure_equals("invalid Analyzer name",name,anlzName);

  t_.commit();
}

//try saving example Target Host
template<>
template<>
void testObj::test<8>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  
  stringstream ss;
  DataBaseID id;
  ss << "SELECT * FROM reported_hosts WHERE id = " << thostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);
  
  r[0]["id_host"].to(id);
  ensure_equals("invalid Host ID",id,hostID);
  
  r[0]["id_alert"].to(id);
  ensure_equals("invalid Alert ID",id,alertID);
 
  t_.commit();
}

//try saving example Destination Host
template<>
template<>
void testObj::test<9>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID dhostID = es_.saveDestinationHost(hostID,alertID,*host);
  
  stringstream ss;
  DataBaseID id;
  ss << "SELECT * FROM reported_hosts WHERE id = " << dhostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["id_host"].to(id);
  ensure_equals("invalid Host ID",id,hostID);

  r[0]["id_alert"].to(id);
  ensure_equals("invalid Alert ID",id,alertID);

  t_.commit();
}

//try saving example Meta Alert
template<>
template<>
void testObj::test<10>(void)
{
  //AlertPtr  alert=makeNewAlert();
  const MetaAlert::Name name("meta alert");
  MetaAlert ma(name,0.22,0.23,makeNewReferenceURL(),created_);
  DataBaseID malertID = es_.saveMetaAlert(ma);
  
  t_.commit();

}

} // namespace tut
