/*
 * TestBaseData.t.cpp
 *
 */
#ifndef INCLUDE_DATAFACADES_STRACCESS_TESTBASEDATA_T_HPP_FILE
#define INCLUDE_DATAFACADES_STRACCESS_TESTBASEDATA_T_HPP_FILE

#include <tut.h>

#include "Commons/Convert.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/ConnectionIOMemory.hpp"
#include "DataFacades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Facades::StrAccess;
using namespace TestHelpers::Persistency;

namespace Facades
{
namespace StrAccess
{

template<typename TTested>
struct TestBaseData: private TestStubs
{
  TestBaseData(void):
    md5_( MD5Sum::createFromString("78b9861f74e15d7d0f077ba22421b8e4") ),
    pid_(42),
    uid_(666),
    url_(new ReferenceURL("evil site", "http://evil.one")),
    process_(new Process("/a/b/c", "somename", &md5_, &pid_, &uid_, "satan", "-v", url_)),
    processNull_(new Process(NULL, "othername", NULL, NULL, NULL, NULL, NULL, (ReferenceURLPtr()) )),
    service_(new Service("servicename", 42, "serviceproto", url_)),
    serviceNull_(new Service("otherservice", 42, Service::Protocol(NULL), ReferenceURLPtr())),
    netmask_( Host::Netmask::from_string("255.255.0.0") ),
    ip_( Host::IP::from_string("1.2.3.4") ),
    host_(new Host( ip_,
                   &netmask_,
                    "Linux",
                    url_,
                    initCollection<Host::Services>(service_, serviceNull_),
                    initCollection<Host::Processes>(process_),
                    "my.dns.name") ),
    hostNull_(new Host(ip_,
                       NULL,
                       NULL,
                       ReferenceURLPtr(),
                       Host::Services(),
                       Host::Processes(),
                       NULL) ),
    analyzer_(new Analyzer(666, "analyzer1", "4.2", "Linux/Debian", &ip_)),
    analyzerNull_(new Analyzer(666, "analyzer2", NULL, NULL, NULL)),
    detectTime_(112233),
    createTime_(556677),
    alert_(new Alert( "alertname",
                      Alert::Analyzers(analyzer_),
                     &detectTime_,
                      createTime_,
                      Severity(SeverityLevel::INFO),
                      0.5,
                      "some text info",
                      initCollection<Alert::Hosts>(host_, hostNull_),
                      initCollection<Alert::Hosts>(host_) )),
    alertNull_(new Alert("alertname",
                         Alert::Analyzers(analyzer_),
                         NULL,
                         createTime_,
                         Severity(SeverityLevel::INFO),
                         0.666,
                         "some text info",
                         Alert::Hosts(),
                         Alert::Hosts() )),
    conn_(new ConnectionIOMemory),
    t_( conn_->createNewTransaction("str_facade_test") ),
    maData_(new MetaAlert("meta alert name", 0.5, -0.5, url_, createTime_, 303)),
    children_( GraphNodePtrNN(new GraphNode(makeNewAlert(), 101, conn_, t_)),
               GraphNodePtrNN(new GraphNode(makeNewAlert(), 102, conn_, t_)) ),
    metaalert_(new GraphNode(maData_, conn_, t_, children_)),
    metaalertLeaf_(new GraphNode( makeNewAlert(), 665, conn_, t_)),
    metaalertNull_(metaalertLeaf_),
    metaalertNode_(metaalert_)
  {
  }

  template<typename C, typename T>
  C initCollection(const T &t1, const T &t2) const
  {
    C c;
    c.push_back(t1);
    c.push_back(t2);
    return c;
  }
  template<typename C, typename T>
  C initCollection(const T &t1) const
  {
    C c;
    c.push_back(t1);
    return c;
  }

  template<typename TData>
  void ensureThrow(const TData &data, const TestParams &p) const
  {
    ensureThrowEx<ExceptionInvalidPath>("call didn't throw on invalid path", data, p);
  }

  template<typename TEx, typename TData>
  void ensureThrowEx(const char *msg, const TData &data, const TestParams &p) const
  {
    try
    {
      TTested::process(data, p);
      tut::fail(msg);
    }
    catch(const TEx &)
    {
      // this is expected
    }
  }

  template<typename TData>
  std::string getProcValue(const TData &data, const char *path)
  {
    TestParams p(Path(path), cb_);
    tut::ensure("error response returned", TTested::process(data, p) );
    return cb_.lastValue_;
  }

  template<typename TData, typename TExp>
  void ensureProc(const char *errMsg, const TData &data, const char *path, const TExp &exp)
  {
    TestParams p(Path(path), cb_);
    tut::ensure("error response returned", TTested::process(data, p) );
    // check
    const std::string expStr=Commons::Convert::to<std::string>(exp);
    tut::ensure_equals(errMsg, cb_.lastValue_, expStr);
  }

  template<typename TData>
  void ensureProcNull(const char *errMsg, const TData &data, const char *path, const std::string &exp)
  {
    TestParams p(Path(path), cb_);
    tut::ensure("non-error response returned", TTested::process(data, p));
    // check
    tut::ensure_equals(errMsg, cb_.lastNullFound_, exp);
  }

  template<typename TData>
  void ensureProcSize(const char *errMsg, const TData &data, const char *path, const size_t exp)
  {
    TestParams p(Path(path), cb_);
    tut::ensure("error response returned", TTested::process(data, p) );
    // check
    tut::ensure_equals(errMsg, cb_.lastSize_, exp);
  }


  TestParams::ResultCallback cb_;

  MD5Sum                     md5_;

  pid_t                      pid_;
  int                        uid_;
  ReferenceURLPtr            url_;
  ProcessPtrNN               process_;
  ProcessPtrNN               processNull_;

  ServicePtrNN               service_;
  ServicePtrNN               serviceNull_;

  Host::Netmask              netmask_;
  Host::IP                   ip_;
  HostPtrNN                  host_;
  HostPtrNN                  hostNull_;

  AnalyzerPtrNN              analyzer_;
  AnalyzerPtrNN              analyzerNull_;

  Timestamp                  detectTime_;
  Timestamp                  createTime_;
  AlertPtrNN                 alert_;
  AlertPtrNN                 alertNull_;

  IO::ConnectionPtrNN        conn_;
  IO::Transaction            t_;
  MetaAlertPtrNN             maData_;
  NodeChildrenVector         children_;
  GraphNodePtrNN             metaalert_;
  GraphNodePtrNN             metaalertLeaf_;
  GraphNodePtrNN             metaalertNull_;
  GraphNodePtrNN             metaalertNode_;
};

} // namespace StrAccess
} // namespace Facades

#endif
