/*
 * TestBaseData.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_TESTBASEDATA_T_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_TESTBASEDATA_T_HPP_FILE

#include <tut.h>

#include "Commons/Convert.hpp"
#include "Persistency/TestBase.t.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

template<typename TTested>
struct TestBaseData: private TestBase
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
    host_( new Host( Host::IP::from_string("1.2.3.4"),
                    &netmask_,
                     "Linux",
                     url_,
                     initCollection<Host::Services>(service_, serviceNull_),
                     initCollection<Host::Processes>(process_),
                     "my.dns.name") ),
    hostNull_( new Host(Host::IP::from_string("1.2.3.4"),
                        NULL,
                        NULL,
                        ReferenceURLPtr(),
                        Host::Services(),
                        Host::Processes(),
                        NULL) )
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
  void ensureThrow(const TData &data, TestParams &p) const
  {
    try
    {
      TTested::process(data, p);
      tut::fail("call didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  template<typename TData, typename TExp>
  void ensureProc(const char *errMsg, const TData &data, const char *path, const TExp &exp)
  {
    TestParams p(Path(path), cb_);
    TTested::process(data, p);
    // check
    const std::string expStr=Commons::Convert::to<std::string>(exp);
    tut::ensure_equals(errMsg, cb_.lastValue_, expStr);
  }

  template<typename TData>
  void ensureProcNull(const char *errMsg, const TData &data, const char *path, const std::string &exp)
  {
    TestParams p(Path(path), cb_);
    TTested::process(data, p);
    // check
    tut::ensure_equals(errMsg, cb_.lastNullFound_, exp);
  }

  template<typename TData>
  void ensureProcSize(const char *errMsg, const TData &data, const char *path, const size_t exp)
  {
    TestParams p(Path(path), cb_);
    TTested::process(data, p);
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
  HostPtrNN                  host_;
  HostPtrNN                  hostNull_;
};

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
