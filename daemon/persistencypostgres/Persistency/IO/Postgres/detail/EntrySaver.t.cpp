/*
 * EntrySaver.t.cpp
 *
 */
#include <tut.h>
#include <boost/algorithm/string/trim.hpp>

#include "Commons/Threads/Thread.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"
#include "Persistency/IO/Postgres/TestConnection.t.hpp"
#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

using namespace std;
using namespace pqxx;
using namespace Persistency;
using namespace Persistency::IO::Postgres;
using namespace Persistency::IO::Postgres::detail;

using boost::algorithm::trim;
using Persistency::IO::Transaction;

namespace
{

const Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                   0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                               } };

template<typename T>
inline pqxx::result execSQL(Transaction &t, const T &sql)
{
    return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()

struct TestClass
{
  TestClass(void):
    name_("some name"),
    analyzer_( new Analyzer(321u, "analyzer name", NULL, NULL, NULL) ),
    analyzers_(analyzer_),
    detected_(1500),
    created_(2900),
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

  Alert::ReportedHosts generateReportedHosts(unsigned int size) const
  {
    Alert::ReportedHosts out;
    for(unsigned int i=0; i<size; ++i)
      out.push_back( makeNewHost() );
    return out;
  }

  Host::Name readHostName(DataBaseID hostID)
  {
    stringstream ss;
    ss << "SELECT * FROM hosts WHERE id = " << hostID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    tut::ensure_equals("invalid size", r.size(), 1u);
    return ReaderHelper<Host::Name>::readAs(r[0]["name"]);
  }

  DataBaseID getID(const Severity s)
  {
    stringstream ss;
    ss<<"SELECT id FROM severities WHERE level="<<s.getLevel().toInt();
    const result r=t_.getAPI<TransactionAPI>().exec(ss);
    tut::ensure_equals("invalid size", r.size(), 1u);
    return ReaderHelper<DataBaseID>::readAs(r[0]["id"]);
  }
  std::string createString(const int &size)
  {
    std::string s;
    for(int i=0;i<size;++i)
      s.push_back('a');
    return s;
  }
  void CreateTempTable()
  {
    execSQL(t_, "CREATE TEMP TABLE tmp"
                "("
                "  s3   char(3)    NULL,"
                "  s16  char(16)   NULL"
                ") ON COMMIT DROP;" );
  }


  const Alert::Name          name_;
  const AnalyzerPtrNN        analyzer_;
  Alert::SourceAnalyzers     analyzers_;
  const Timestamp            detected_;
  const Timestamp            created_;
  const Severity             severity_;
  const Certainty            certanity_;
  const std::string          description_;
  const Alert::ReportedHosts sourceHosts_;
  const Alert::ReportedHosts targetHosts_;


  TestDBAccess        tdba_;
  IDCachePtrNN        idCache_;
  DBHandle            dbh_;
  IO::ConnectionPtrNN conn_;
  Transaction         t_;
  EntrySaver          es_;

  const pid_t        pid_;
  const int          uid_;
  ReferenceURLPtrNN  url_;
  const char        *md5Str_;
  const MD5Sum       md5_;
  const Process      proc_;
  const Process      procnn_;

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
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(112u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID alertID = es_.saveAlert(a);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  // save data in table alert_analyzers
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID hostID  = es_.saveTargetHost(alertID, *host);
  const DataBaseID procID  = es_.saveProcess(hostID, proc_);

  stringstream ss;
  string path, name;
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);

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
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(303u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  // save data in table alert_analyzers
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID hostID  = es_.saveTargetHost(alertID, *host);
  const DataBaseID id1=es_.saveProcess(hostID, proc_);
  const DataBaseID id2=es_.saveProcess(hostID, proc_);
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
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);

  const Analyzer anlz(102u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID = es_.saveAnalyzer(anlz);
  const DataBaseID alrtID = es_.saveAlert(a);
  es_.saveAlertToAnalyzers(alrtID, anlzID);
  stringstream ss;

  ss << "SELECT * FROM alerts WHERE id = " << alrtID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  ensure_equals("invalid name",
                ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                name_.get());

  ensure_equals("invalid detect time",
                timestampFromString( ReaderHelper<string>::readAsNotNull(r[0]["detect_time"]) ),
                detected_);

  ensure_equals("invalid create time",
                timestampFromString( ReaderHelper<string>::readAsNotNull(r[0]["create_time"]) ),
                created_
                );

  ensure_equals("invalid severity ID",
                ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_severity"]),
                getID( a.getSeverity() ) );

  ensure_equals("invalid certanity",
                ReaderHelper<double>::readAsNotNull(r[0]["certanity"]),
                certanity_.get());

  ensure_equals("invalid description",
                ReaderHelper<string>::readAsNotNull(r[0]["description"]),
                description_);

  t_.commit();
}

// try saving example Service
template<>
template<>
void testObj::test<5>(void)
{
  const Service ti("mail daemon", 25, "smtp", makeNewReferenceURL() );
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(101u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID hostID  = es_.saveTargetHost(alertID, *host);
  const DataBaseID servID  = es_.saveService(hostID, ti);

  stringstream ss;
  string protocol, name;

  ss << "SELECT * FROM services WHERE id = " << servID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  r[0]["name"].to(name);
  trim(name);
  ensure_equals("invalid name",ti.getName().get()  ,name );

  ensure_equals("invalid port", ti.getPort().get() ,ReaderHelper<uint16_t>::readAsNotNull(r[0]["port"]) );

  r[0]["protocol"].to(protocol);
  trim(protocol);
  ensure_equals("invalid protocol", ti.getProtocol().get(), protocol);

  t_.commit();
}

// try saving example Analyzer with NULL Version, OS and IP
template<>
template<>
void testObj::test<6>(void)
{
  const Analyzer   a(999u, "analyzer2", NULL, NULL, NULL);
  const DataBaseID anlzID = es_.saveAnalyzer(a);

  stringstream ss;
  string       tmp;
  ss << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["sys_id"].to(tmp);
  ensure_equals("invalid ID", tmp, "999");
  r[0]["name"].to(tmp);
  ensure_equals("invalid Analyzer name", tmp, "analyzer2");

  ensure("Analyzer Version is not NULL", r[0]["version"].is_null() );

  ensure("Analyzer OS is not NULL", r[0]["os"].is_null() );

  ensure("Analyzer IP is not NULL", r[0]["ip"].is_null() );
  t_.commit();
}

// try saving example Analyzer
template<>
template<>
void testObj::test<7>(void)
{
  const string                    anlzName("analyzer3");
  const Analyzer::Version         ver("v1.2.3");
  const Analyzer::OperatingSystem os("Linux 2.6.66");
  const Analyzer::IP              ip( Analyzer::IPv4::from_string("1.2.3.4") );
  const Analyzer                  a(262u, anlzName, ver, os, &ip);
  const DataBaseID                anlzID = es_.saveAnalyzer(a);

  stringstream ss;
  string tmp, version, os_name, ip_adress;

  ss << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["sys_id"].to(tmp);
  ensure_equals("invalid Analyzer's ID", tmp, "262");
  r[0]["name"].to(tmp);
  ensure_equals("invalid Analyzer's name", tmp, anlzName);

  r[0]["version"].to(version);
  trim(version);
  ensure_equals("invaluid Analyzer Version", ver.get(), version);

  r[0]["os"].to(os_name);
  trim(os_name);
  ensure_equals("invalid Analyzer OS", os.get(), os_name);

  r[0]["ip"].to(ip_adress);
  trim(ip_adress);
  ensure_equals("invalid Analyzer IP", ip.to_string(), ip_adress);
  t_.commit();
}

// try saving example Target Host
template<>
template<>
void testObj::test<8>(void)
{
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(22u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID hostID = es_.saveTargetHost(alertID, *host);

  stringstream ss;
  DataBaseID id;
  ss << "SELECT * FROM hosts WHERE id = " << hostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["id_alert"].to(id);
  ensure_equals("invalid Alert ID",id,alertID);

  t_.commit();
}

// try saving example Source Host
template<>
template<>
void testObj::test<9>(void)
{
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(88u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID dhostID = es_.saveSourceHost(alertID, *host);

  stringstream ss;
  DataBaseID id;
  ss << "SELECT * FROM hosts WHERE id = " << dhostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

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
  MetaAlert ma(name, 0.22, 0.23, makeNewReferenceURL(), created_, 42u);
  const DataBaseID malertID = es_.saveMetaAlert(ma);

  Persistency::MetaAlert::ID::Numeric numericID;
  double                              delta;
  string                              mAlertName;
  string                              time;
  stringstream                        ss;

  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["sys_id"].to(numericID);
  ensure_equals("invalid system-ID", numericID, 42u);

  r[0]["severity_delta"].to(delta);
  ensure_equals("invalid severity delta", delta, 0.22);

  r[0]["certainty_delta"].to(delta);
  ensure_equals("invalid certanity delta", delta, 0.23);

  r[0]["name"].to(mAlertName);
  ensure_equals("invalid name", mAlertName, "meta alert");

  r[0]["create_time"].to(time);
  ensure_equals("invalid created time", timestampFromString(time), created_);

  t_.commit();
}

//try save alert with NULL detected time
template<>
template<>
void testObj::test<11>(void)
{
  const Alert a(name_, analyzers_, NULL, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);

  const Analyzer anlz(123u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID = es_.saveAnalyzer(anlz);
  const DataBaseID alrtID = es_.saveAlert(a);
  // save data in alert_analyzers
  es_.saveAlertToAnalyzers(alrtID, anlzID);
  stringstream ss;
  string name, time, description;
  DataBaseID id;
  double certanity;

  ss << "SELECT * FROM alerts WHERE id = " << alrtID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["name"].to(name);
  ensure_equals("invalid name",name_.get(),name);


  r[0]["detect_time"].to(time);
  ensure_equals("invalid detect time", "", time);

  r[0]["create_time"].to(time);
  ensure_equals("invalid create time", created_, timestampFromString(time) );

  r[0]["id_severity"].to(id);
  ensure_equals("invalid severity ID", id, getID( a.getSeverity() ) );

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
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(123u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  // save data in alert_analyzers
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID thostID = es_.saveTargetHost(alertID, *host);
  const DataBaseID procID  = es_.saveProcess(thostID, procnn_);

  stringstream ss;
  string path, name, md5;
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  r[0]["path"].to(path);
  ensure_equals("invalid path",proc_.getPath().get() ,path);

  r[0]["name"].to(name);
  ensure_equals("invalid name",proc_.getName().get() ,name);

  r[0]["md5"].to(md5);
  trim(md5);
  ensure_equals("invalid md5 sum",procnn_.getMD5()->get(), md5);

  t_.commit();
}

// try save Process with NULL ReferenceURL
template<>
template<>
void testObj::test<13>(void)
{
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  ReferenceURLPtr url;
  const Process proc("/a/b/c/d", "some.proc", NULL, &pid_, &uid_, "johndoe", "-a -b -c", url);
  const Analyzer anlz(1111u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  // save data in alert_analyzers
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID thostID = es_.saveTargetHost(alertID, *host);
  const DataBaseID procID  = es_.saveProcess(thostID, proc);

  stringstream ss;
  string username, arguments;
  int pid, uid;
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);


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

// try save Process with non-NULL ReferenceURL
template<>
template<>
void testObj::test<14>(void)
{
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Process proc("/a/b/c/d", "some.proc", &md5_, &pid_, &uid_, "johndoe", "-a -b -c", url_);
  const Analyzer anlz(66u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  const DataBaseID alertID = es_.saveAlert(a);
  // save data in alert_analyzers
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID thostID = es_.saveTargetHost(alertID, *host);
  const DataBaseID procID  = es_.saveProcess(thostID, proc);

  stringstream ss;
  string username, arguments, url_name, url_str;
  int pid, uid;
  ss << "SELECT * FROM procs WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

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

//test saving Host with NULL name
template<>
template<>
void testObj::test<15>(void)
{
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  const Host  h(  Host::IPv4::from_string("1.2.3.4"),
                 &mask4_,
                  "myos",
                  makeNewReferenceURL(),
                  Host::ReportedServices(),
                  Host::ReportedProcesses(),
                  NULL );
  const DataBaseID alertID = es_.saveAlert(a);
  const DataBaseID hostID  = es_.saveSourceHost(alertID, h);

  stringstream ss;


  ss << "SELECT * FROM hosts WHERE id = " << hostID << ";";

  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  ensure("Host name is not NULL",r[0]["name"].is_null() );
  t_.commit();
}

// trying save max value length to the data base
template<>
template<>
void testObj::test<16>(void)
{
  CreateTempTable();
  stringstream ss;
  string       s;
  ss << "INSERT INTO tmp(s3, s16) VALUES(";
  Appender::append(ss, createString(3) );
  ss << ", ";
  Appender::append(ss, createString(16) );
  ss << ")";
  t_.getAPI<TransactionAPI>().exec(ss);
  ss.str("");
  ss << "SELECT * FROM tmp;";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size() ,1u);
  r[0]["s3"].to(s);
  ensure_equals("invalid read data size", s.size(), 3u);
  s.clear();
  r[0]["s16"].to(s);
  ensure_equals("invalid read data size", s.size(), 16u);
}

// try save service with NULL reference URL
template<>
template<>
void testObj::test<17>(void)
{
  ReferenceURLPtr url;
  const Service ti("mail daemon", 25, "smtp", url );
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  HostPtr host=makeNewHost();
  const Analyzer anlz(123u, "analyzer1", NULL, NULL, NULL);
  const DataBaseID alertID = es_.saveAlert(a);
  const DataBaseID anlzID  = es_.saveAnalyzer(anlz);
  es_.saveAlertToAnalyzers(alertID, anlzID);
  const DataBaseID thostID = es_.saveTargetHost(alertID, *host);
  const DataBaseID servID = es_.saveService(thostID, ti);

  stringstream ss;

  ss << "SELECT * FROM services WHERE id = " << servID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  ensure("reference URL is not NULL", r[0]["id_ref"].is_null() );

  t_.commit();
}

// try save Meta Alert with NULL reference URL.
template<>
template<>
void testObj::test<18>(void)
{
  const MetaAlert::Name name("meta alert");
  ReferenceURLPtr url;
  MetaAlert ma(name, 0.22,0.23, url , created_, 913u);
  const DataBaseID malertID = es_.saveMetaAlert(ma);

  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  ensure("Meta Alert reference URL is not NULL",r[0]["id_ref"].is_null() );

  t_.commit();

}

//test saving two identical Analyzers
template<>
template<>
void testObj::test<19>(void)
{
  const string                    anlzName("analyzer3");
  const Analyzer::ID              id(987654321u);
  const Analyzer::Version         ver("v1.2.3");
  const Analyzer::OperatingSystem os("Linux 2.6.66");
  const Analyzer::IP              ip( Analyzer::IPv4::from_string("1.2.3.4") );
  const Analyzer                  a1(id, anlzName, ver, os, &ip);
  const Analyzer                  a2(id, anlzName, ver, os, &ip);
  es_.saveAnalyzer(a1);
  es_.saveAnalyzer(a2);

  stringstream ss;
  ss << "SELECT * FROM analyzers WHERE name = ";
  Appender::append(ss, anlzName );
  ss << "AND sys_id = ";
  Appender::append(ss, id.get() );
  ss << "AND version = ";
  Appender::append(ss, ver.get() );
  ss << " AND os = ";
  Appender::append(ss, os.get() );
  ss << "AND ip = ";
  Appender::append(ss, ip.to_string() );
  ss << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);
  t_.commit();
}

// trying save MetaAlert as triggered
template<>
template<>
void testObj::test<20>(void)
{
  const string TriggerName("some trigger name");
  const MetaAlert::Name name("meta alert");
  MetaAlert ma(name, 0.22, 0.23, makeNewReferenceURL(), created_, 555u);
  const DataBaseID malertID = es_.saveMetaAlert(ma);
  es_.markMetaAlertAsUsed(malertID);
  stringstream ss;
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
  }
  es_.markMetaAlertAsTriggered(malertID, TriggerName);
  ss.str("");
  {
    ss << "SELECT * FROM meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
    result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
    ensure_equals("invalid trigger name", ReaderHelper<string>::readAsNotNull(r[0]["trigger_name"]), TriggerName);
  }
}

// trying save MetaAlert as used
template<>
template<>
void testObj::test<21>(void)
{
  const MetaAlert::Name name("meta alert");
  MetaAlert ma(name, 0.22, 0.23, makeNewReferenceURL(), created_, 23u);
  const DataBaseID malertID = es_.saveMetaAlert(ma);
  es_.markMetaAlertAsUsed(malertID);
  stringstream ss;
  ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size",r.size(), 1u);
  t_.commit();
}

// trying save MetaAlert as unused
template<>
template<>
void testObj::test<22>(void)
{
  const MetaAlert::Name name("meta alert");
  MetaAlert ma(name, 0.22, 0.23, makeNewReferenceURL(), created_, 404u);
  const DataBaseID malertID = es_.saveMetaAlert(ma);
  es_.markMetaAlertAsUsed(malertID);
  stringstream ss;
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 1u);
  }
  es_.markMetaAlertAsUnused(malertID);
  ss.str("");
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid size",r.size(), 0);
  }

}

// test seting host name
template<>
template<>
void testObj::test<23>(void)
{
  const string     hname("some.host.com");
  const Host::Name hostName(hname);
  const Host       h( Host::IPv4::from_string("1.2.3.4"),
                      &mask4_,
                      "myos",
                      makeNewReferenceURL(),
                      Host::ReportedServices(),
                      Host::ReportedProcesses(),
                      NULL );
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  const DataBaseID alertID = es_.saveAlert(a);
  const DataBaseID hostID  = es_.saveSourceHost(alertID, h);
  ensure("Host name is not NULL", readHostName(hostID).get() == NULL );
  // trying set Host name
  es_.setHostName(hostID, hostName);
  string name( readHostName(hostID).get() );
  trim(name);
  ensure_equals("invalid host name",  name, hname);
  t_.commit();
}

// test saving host with NULL name and NULL mask
template<>
template<>
void testObj::test<24>(void)
{
  const Host h(  Host::IPv4::from_string("1.2.3.4"),
                 NULL,
                 "myos",
                 makeNewReferenceURL(),
                 Host::ReportedServices(),
                 Host::ReportedProcesses(),
                 NULL );
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sourceHosts_, targetHosts_);
  const DataBaseID alertID = es_.saveAlert(a);
  const DataBaseID hostID  = es_.saveSourceHost(alertID, h);

  stringstream ss;
  ss << "SELECT * FROM hosts WHERE id = " << hostID << ";";
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);

  ensure("Host name is not NULL",r[0]["name"].is_null() );
  ensure("Mask is not NULL",r[0]["mask"].is_null() );
  t_.commit();
}

// test for bug when trying to save NULL reference URL
template<>
template<>
void testObj::test<25>(void)
{
  HostPtrNN h( new Host( Host::IPv4::from_string("1.2.3.4"),
                         NULL,
                         "myos",
                         ReferenceURLPtr(),
                         Host::ReportedServices(),
                         Host::ReportedProcesses(),
                         NULL ) );
  Alert::ReportedHosts sh;
  sh.push_back(h);
  // save some alert
  const Alert a(name_, analyzers_, &detected_, created_, severity_, certanity_,
                description_, sh, targetHosts_);
  const DataBaseID alertID = es_.saveAlert(a);
  // save data in table alert_analyzers
  const DataBaseID thostID = es_.saveSourceHost(alertID, *h);   // original bug thrown here

  // check if entry is really there
  stringstream ss;
  ss << "SELECT * FROM hosts WHERE"
     << " id = " << thostID
     << " AND id_alert =  " << alertID;
  const result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);
  t_.commit();
}

// trying save MetaAlert as triggered and then remove saved Meta Alert from triggered
template<>
template<>
void testObj::test<26>(void)
{
  const string          triggerName("some trigger name");
  const MetaAlert::Name name("meta alert");
  MetaAlert             ma(name, 0.22, 0.23, makeNewReferenceURL(), created_, 101u);
  const DataBaseID      malertID = es_.saveMetaAlert(ma);
  es_.markMetaAlertAsUsed(malertID);
  stringstream ss;
  {
    ss << "SELECT * FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid number of used meta-alerts in data base", r.size(), 1u);
  }
  es_.markMetaAlertAsTriggered(malertID, triggerName);
  ss.str("");
  {
    ss << "SELECT * FROM meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid number of trigered meta-alerts in data base", r.size(), 1u);
    ensure_equals("invalid trigger name", ReaderHelper<string>::readAsNotNull(r[0]["trigger_name"]), triggerName);
  }
  es_.markMetaAlertAsUnused(malertID);
  ss.str("");
  {
    ss << "SELECT * FROM meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("invalid number of trigered meta-alerts in data base", r.size(), 0u);
  }
}

// try saving new value with saveConfigParameter
template<>
template<>
void testObj::test<27>(void)
{
  // save
  es_.saveConfigParameter("owner1", "key1", "value1");
  // check
  {
    stringstream ss;
    ss << "SELECT * FROM config WHERE owner='owner1' AND key='key1' AND value='value1'";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("entry not saved", r.size(), 1u);
  }
}

// try saving new value with saveConfigParameter - NULL owner
template<>
template<>
void testObj::test<28>(void)
{
  // save
  es_.saveConfigParameter(NULL, "key1", "value1");
  // check
  {
    stringstream ss;
    ss << "SELECT * FROM config WHERE owner IS NULL AND key='key1' AND value='value1'";
    const result r = t_.getAPI<TransactionAPI>().exec(ss);
    ensure_equals("entry not saved", r.size(), 1u);
  }
}

//test saving two identical Analyzers with NULLs
template<>
template<>
void testObj::test<29>(void)
{
  const string                    anlzName("analyzer3");
  const Analyzer::ID              id(987654321u);
  const Analyzer                  a1(id, anlzName, NULL, NULL, NULL);
  const Analyzer                  a2(id, anlzName, NULL, NULL, NULL);
  es_.saveAnalyzer(a1);
  es_.saveAnalyzer(a2);

  stringstream ss;
  ss << "SELECT * FROM analyzers WHERE name = ";
  Appender::append(ss, anlzName );
  ss << "AND sys_id = ";
  Appender::append(ss, id.get() );
  ss << "AND version IS NULL ";
  ss << "AND os IS NULL ";
  ss << "AND ip IS NULL";
  ss << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ensure_equals("invalid size", r.size(), 1u);
  t_.commit();
}


namespace
{
struct WriterThread
{
  explicit WriterThread(std::string *out, const char *key="some key"):
    key_(key),
    out_(out)
  {
    tut::ensure("NULL output", out_!=NULL);
    tut::ensure("invalid initial value", *out_=="");
  }

  void operator()(void)
  {
    try
    {
      IDCachePtrNN        idCache(new IDCache);
      DBHandle            dbh(TestConnection::makeParams(), idCache);
      IO::ConnectionPtrNN conn( makeConnection() );

      for(int i=0; i<200; ++i)
      {
        Transaction t( conn->createNewTransaction("read_write_stress_test") );
        EntrySaver  es(t, dbh);
        es.saveConfigParameter( "some owner", key_, "some value: " + Commons::Convert::to<std::string>(i) );
        boost::this_thread::yield();        // increase risk of race condition, if present
        t.commit();
      }
    }
    catch(const std::exception &ex)
    {
      *out_=ex.what();
    }
  }

  const char  *key_;
  std::string *out_;
}; // struct WriterThread

} // unnamed namespace

// test if there is no races when multiple write from multiple threads occures
// at the same time (this checks how write is organized).
// NOTE: this code tests for a bug that caused unique constraint volation.
template<>
template<>
void testObj::test<30>(void)
{
  std::string              out[5];
  Commons::Threads::Thread th1( (WriterThread(&out[0])) );
  Commons::Threads::Thread th2( (WriterThread(&out[1])) );
  Commons::Threads::Thread th3( (WriterThread(&out[2])) );
  Commons::Threads::Thread th4( (WriterThread(&out[3])) );
  Commons::Threads::Thread th5( (WriterThread(&out[4])) );
  // wait until they finish
  th1->join();
  th2->join();
  th3->join();
  th4->join();
  th5->join();
  // check output
  ensure_equals("thread 1 failed", out[0], "");
  ensure_equals("thread 2 failed", out[1], "");
  ensure_equals("thread 3 failed", out[2], "");
  ensure_equals("thread 4 failed", out[3], "");
  ensure_equals("thread 5 failed", out[4], "");
}

// test if there is no races when multiple write from multiple threads occures
// at the same time, but to different keys (i.e. entries).
// NOTE: this code tests for a bug that can appear in implementation from 2010.08.30.
template<>
template<>
void testObj::test<31>(void)
{
  std::string              out[6];
  Commons::Threads::Thread th1( (WriterThread(&out[0], "k1")) );
  Commons::Threads::Thread th2( (WriterThread(&out[1], "k2")) );
  Commons::Threads::Thread th3( (WriterThread(&out[2], "k3")) );
  Commons::Threads::Thread th4( (WriterThread(&out[3], "k4")) );
  Commons::Threads::Thread th5( (WriterThread(&out[4], "k5")) );
  Commons::Threads::Thread th6( (WriterThread(&out[5], "k6")) );
  // wait until they finish
  th1->join();
  th2->join();
  th3->join();
  th4->join();
  th5->join();
  th6->join();
  // check output
  ensure_equals("thread 1 failed", out[0], "");
  ensure_equals("thread 2 failed", out[1], "");
  ensure_equals("thread 3 failed", out[2], "");
  ensure_equals("thread 4 failed", out[3], "");
  ensure_equals("thread 5 failed", out[4], "");
  ensure_equals("thread 6 failed", out[5], "");
}

} // namespace tut
