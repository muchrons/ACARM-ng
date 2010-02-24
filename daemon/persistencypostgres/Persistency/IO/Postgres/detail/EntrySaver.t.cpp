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
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include <boost/algorithm/string.hpp>

using Persistency::IO::Transaction;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;
using namespace std;
using namespace pqxx;
using boost::posix_time::from_iso_string;
using boost::posix_time::time_from_string;
using boost::algorithm::trim;
/* TODO: THIS IS COMMENTED OUT SINCE IMPLEMENTATION OF ANALYZER CHANGED - UPDATE THIS CODE ASAP
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
    analyzer_( new Analyzer("analyzer name", NULL, NULL, NULL) ),
    detected_(from_iso_string("2001109T231100")),
    created_(from_iso_string("20011010T231100")),
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
    md5Str_("01234567890123456789012345678901"),
    md5_( MD5Sum::createFromString(md5Str_) ),
    proc_( "/a/b/c/d",
           "some.proc",
           NULL,
           &pid_,
           &uid_,
           "johndoe",
           "-a -b -c",
           url_ ),
    procnn_( "/a/b/c/d",
            "some.proc",
            &md5_,
            &pid_,
            &uid_,
            "johndoe",
            "-a -b -c",
            url_ ),
    mask4_( Host::Netmask_v4(mask4_bytes) ),
    mask6_( Host::Netmask_v6(mask6_bytes) )
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
  const AnalyzerPtrNN        analyzer_;
  const Timestamp            detected_;
  const Timestamp            created_;
  const Severity             severity_;
  const Certainty            certanity_;
  const std::string          description_;
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
  const char        *md5Str_;
  const MD5Sum      md5_;
  const Process     proc_;
  const Process     procnn_;

  const Host::Netmask_v4 mask4_;
  const Host::Netmask_v6 mask6_;

};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/EntrySaver");
} // unnamed namespace


namespace tut
{

// try saving example Process with NULL MD5 sum- smoke test
template<>
template<>
void testObj::test<1>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID procID  = es_.saveProcess(thostID, proc_);

  stringstream ss;
  string path, name;
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["path"].to(path);
  ensure_equals("invalid path",proc_.getPath().get() ,path);

  r[0]["name"].to(name);
  ensure_equals("invalid name",proc_.getName().get() ,name);

  ensure("invalid md5 sum",r[0]["md5"].is_null());

  t_.commit();
}

// check returned IDs
template<>
template<>
void testObj::test<2>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID id1=es_.saveProcess(thostID, proc_);
  const DataBaseID id2=es_.saveProcess(thostID, proc_);
  ensure("invalid ids returned", id1<id2);
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
  // TODO: these variables should be const:
  DataBaseID hostID = es_.saveHostData(*host);
  DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alrtID = es_.saveAlert(anlzID,a);

  stringstream ss;
  string name, time, description;
  DataBaseID id;
  double certanity;

  ss << "SELECT * FROM alerts WHERE id = " << alrtID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["name"].to(name);
  ensure_equals("invalid name",name_.get(),name);

  r[0]["id_analyzer"].to(id);
  ensure_equals("invalid analyzer ID",anlzID,id);

  r[0]["detect_time"].to(time);
  ensure_equals("invalid detect time",detected_,time_from_string(time));

  r[0]["create_time"].to(time);
  ensure_equals("invalid create time",created_,time_from_string(time));

  r[0]["id_severity"].to(id);
  ensure_equals("invalid severity ID",a.getSeverity().getLevel().toInt(),id);

  r[0]["certanity"].to(certanity);
  ensure_equals("invalid certanity",certanity_.get(),certanity);

  r[0]["description"].to(description);
  ensure_equals("invalid description",description_,description);

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
  // TODO: these variables should be const:
  DataBaseID hostID  = es_.saveHostData(*host);
  DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  DataBaseID alertID = es_.saveAlert(anlzID,a);
  DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  DataBaseID servID = es_.saveService(thostID,ti);

  stringstream ss;
  string protocol;
  string name;

  // TODO: 'port' variable declarationshuld be moved next to usage place.
  //       note that it can be used within a scope to minimize it's life-time
  int port;
  ss << "SELECT * FROM services WHERE id = " << servID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  r[0]["name"].to(name);
  trim(name);
  ensure_equals("invalid name",ti.getName().get()  ,name );

  r[0]["port"].to(port);
  ensure_equals("invalid port",ti.getPort(),port);

  r[0]["protocol"].to(protocol);
  trim(protocol);
  ensure_equals("invalid protocol", ti.getProtocol().get(), protocol);

  t_.commit();
}

// try saving example Analyzer
template<>
template<>
void testObj::test<6>(void)
{
  HostPtr          host=makeNewHost();
  const Analyzer   a("analyzer2", host);
  const DataBaseID hostID = es_.saveHostData(*host);
  const DataBaseID anlzID = es_.saveAnalyzer(&hostID,a);

  stringstream ss;
  string       name;
  DataBaseID   id;
  ss << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["name"].to(name);
  ensure_equals("invalid Analyzer name",name,"analyzer2");

  r[0]["id_host"].to(id);
  ensure_equals("invalid Host ID",hostID,id);

  t_.commit();
}

// try saving example Analyzer with NULL id_host
template<>
template<>
void testObj::test<7>(void)
{
  const string anlzName("analyzer3");
  // TODO: i'd suggest passing NULL host-ptr here, for better readability
  const Analyzer a(anlzName,makeNewHost());
  const DataBaseID anlzID = es_.saveAnalyzer(NULL,a);

  stringstream ss;
  string name, id;

  ss << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["name"].to(name);
  ensure_equals("invalid Analyzer name",name,anlzName);

  ensure("id_host is not NULL",r[0]["id_host"].is_null());

  t_.commit();
}

// try saving example Target Host
template<>
template<>
void testObj::test<8>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  // TODO: these variables should be const:
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);

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

// try saving example Source Host
template<>
template<>
void testObj::test<9>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID dhostID = es_.saveSourceHost(hostID,alertID,*host);

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
  const MetaAlert::Name name("meta alert");
  MetaAlert ma(name,0.22,0.23,makeNewReferenceURL(),created_);
  const DataBaseID malertID = es_.saveMetaAlert(ma);

  stringstream ss;
  double delta;
  string mAlertName;
  string time;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["severity_delta"].to(delta);
  ensure_equals("invalid severity delta",0.22,delta);

  r[0]["certanity_delta"].to(delta);
  ensure_equals("invalid certanity delta",0.23,delta);

  r[0]["name"].to(mAlertName);
  ensure_equals("invalid name","meta alert",mAlertName);

  r[0]["create_time"].to(time);
  ensure_equals("invalid created time", created_, time_from_string(time));

  t_.commit();
}

//try save alert with NULL detected time
template<>
template<>
void testObj::test<11>(void)
{
  const Alert a(name_, analyzer_, NULL, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID = es_.saveHostData(*host);
  const DataBaseID anlzID = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alrtID = es_.saveAlert(anlzID,a);

   stringstream ss;
  string name, time, description;
  DataBaseID id;
  double certanity;

  ss << "SELECT * FROM alerts WHERE id = " << alrtID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["name"].to(name);
  ensure_equals("invalid name",name_.get(),name);

  r[0]["id_analyzer"].to(id);
  ensure_equals("invalid analyzer ID",anlzID,id);

  r[0]["detect_time"].to(time);
  ensure_equals("invalid detect time", "", time);

  r[0]["create_time"].to(time);
  ensure_equals("invalid create time",created_, time_from_string(time));

  r[0]["id_severity"].to(id);
  ensure_equals("invalid severity ID",a.getSeverity().getLevel().toInt(),id);

  r[0]["certanity"].to(certanity);
  ensure_equals("invalid certanity",certanity_.get(),certanity);

  r[0]["description"].to(description);
  ensure_equals("invalid description",description_,description);

  t_.commit();
}

// try saving example Process with non-NULL MD5 sum- smoke test
template<>
template<>
void testObj::test<12>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID procID  = es_.saveProcess(thostID, procnn_);

  stringstream ss;
  string path, name, md5;
  string md5_(procnn_.getMD5()->get());
  md5_.resize(32,' ');
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["path"].to(path);
  ensure_equals("invalid path",proc_.getPath().get() ,path);

  r[0]["name"].to(name);
  ensure_equals("invalid name",proc_.getName().get() ,name);

  r[0]["md5"].to(md5);
  ensure_equals("invalid md5 sum",md5_, md5);

  t_.commit();
}


// TODO: test saving Process with NULL ReferenceURL

template<>
template<>
void testObj::test<13>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  ReferenceURLPtr url;
  const Process proc("/a/b/c/d", "some.proc", NULL, &pid_, &uid_, "johndoe", "-a -b -c", url);
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID procID  = es_.saveProcess(thostID, proc);

  stringstream ss;
  string username, arguments;
  int pid, uid;
  ss << "SELECT * FROM reported_procs WHERE id_proc = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);


  r[0]["username"].to(username);
  trim(username);
  ensure_equals("invalid name", proc.getUsername().get() ,username);

  r[0]["arguments"].to(arguments);
  ensure_equals("invalid arguments", *proc.getParameters(), arguments);

  ensure("non-NULL ReferenceURL",r[0]["id_ref"].is_null());

  r[0]["uid"].to(uid);
  ensure_equals("invalid uid number",  *proc.getUID(), uid);

  r[0]["pid"].to(pid);
  ensure_equals("invalid pid number",  *proc.getPID(), pid);

  t_.commit();
}


// TODO: test saving Process with non-NULL ReferenceURL
template<>
template<>
void testObj::test<14>(void)
{
  const Alert a(name_, analyzer_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Process proc("/a/b/c/d", "some.proc", &md5_, &pid_, &uid_, "johndoe", "-a -b -c", url_);
  const Analyzer anlz("analyzer1", host);
  const DataBaseID hostID  = es_.saveHostData(*host);
  const DataBaseID anlzID  = es_.saveAnalyzer(&hostID,anlz);
  const DataBaseID alertID = es_.saveAlert(anlzID,a);
  const DataBaseID thostID = es_.saveTargetHost(hostID,alertID,*host);
  const DataBaseID procID  = es_.saveProcess(thostID, proc);

  stringstream ss;
  string username, arguments, url_name, url_str;
  int pid, uid;
  ss << "SELECT * FROM reported_procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(),1);

  r[0]["username"].to(username);
  trim(username);
  ensure_equals("invalid name",proc.getUsername().get() ,username);

  r[0]["arguments"].to(arguments);
  ensure_equals("invalid arguments", *proc.getParameters(), arguments);

  ensure("non-NULL ReferenceURL",!r[0]["id_ref"].is_null());

  r[0]["uid"].to(uid);
  ensure_equals("invalid uid number",  *proc.getUID(), uid);

  r[0]["pid"].to(pid);
  ensure_equals("invalid pid number",  *proc.getPID(), pid);

  t_.commit();



}

// TODO: test saving Host with NULL name
template<>
template<>
void testObj::test<15>(void)
{
}

// TODO: test saving meta-alert with NULL reference url
template<>
template<>
void testObj::test<16>(void)
{
}

// TODO: test saving service with NULL reference URL
template<>
template<>
void testObj::test<17>(void)
{
}

// TODO: add tests to check if max/min-length data types does fill in data base.
template<>
template<>
void testObj::test<18>(void)
{
}

} // namespace tut
*/
